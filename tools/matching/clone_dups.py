#!/usr/bin/env python3
"""Clone matched C to byte-identical twin functions.

Two functions are twins when their bytes are equal after masking relocated
fields (from relocs.txt) and they have the same mode and relocation layout.
For every twin B of a matched function A (A has a registered src file), the
C of A is copied with symbol names remapped: A's name -> B's name, and each
relocation target of A -> the target at the same offset in B. The result is
written to src/<dir>/<B>.c and must then pass match.py (run by integrate.py);
files that fail are deleted.

    python tools/matching/clone_dups.py [--dir clones]
"""
from __future__ import annotations

import argparse
import bisect
import re
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "matching"))
from project import modules, parse_symbols  # noqa: E402
from compiler_sweep import module_bytes  # noqa: E402

REL = re.compile(r"from:0x([0-9a-f]+) kind:(\S+) to:0x([0-9a-f]+)(?: add:(\S+))? module:(\S+)")


def target_module(spec: str) -> str | None:
    if spec in ("main", "itcm", "dtcm"):
        return spec
    m = re.match(r"overlays?\((\d+)", spec)
    return f"ov{int(m.group(1)):03d}" if m else None


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--dir", default="clones")
    ap.add_argument("--version", default="eur")
    args = ap.parse_args()

    mods = module_bytes(args.version)
    mlist = modules(args.version)
    names: dict[str, dict[int, str]] = {}      # module -> addr -> symbol name
    funcs = []
    for m in mlist:
        nm = {}
        for s in parse_symbols(m["symbols_path"]):
            nm.setdefault(s["addr"], s["name"])
            if s["kind"] == "function" and s["size"]:
                funcs.append((m["name"], s))
        names[m["name"]] = nm
    relocs: dict[str, dict[int, tuple[str, int, str]]] = {}
    for m in mlist:
        d = {}
        for line in (m["symbols_path"].parent / "relocs.txt").read_text().splitlines():
            r = REL.match(line)
            if r:
                d[int(r[1], 16)] = (r[2], int(r[3], 16), r[5])
        relocs[m["name"]] = d

    def signature(mod: str, s: dict):
        base, blob = mods[mod]
        a = s["addr"]
        b = bytearray(blob[a - base:a - base + s["size"]])
        layout = []
        for o in range(0, len(b), 2):
            r = relocs[mod].get(a + o)
            if not r:
                continue
            kind = r[0]
            layout.append((o, kind))
            n = 3 if kind in ("arm_call", "arm_branch", "arm_call_thumb") else 4
            b[o:o + n] = b"\0" * n
        return (s["mode"], bytes(b), tuple(layout))

    groups = defaultdict(list)
    for mod, s in funcs:
        groups[signature(mod, s)].append((mod, s))

    # registered sources: function name -> src path
    src_of = {}
    for p in (ROOT / "config").rglob("delinks.txt"):
        for f in re.findall(r"^(src/\S+\.c):$", p.read_text(), re.M):
            stem = Path(f).stem
            src_of[stem] = f
    existing = {p.stem for p in (ROOT / "src").rglob("*.c")}

    out_dir = ROOT / "src" / args.dir
    out_dir.mkdir(parents=True, exist_ok=True)
    made = 0
    for members in groups.values():
        if len(members) < 2:
            continue
        donors = [(mod, s) for mod, s in members if s["name"] in src_of]
        if not donors:
            continue
        dmod, ds = donors[0]
        text = (ROOT / src_of[ds["name"]]).read_text()
        for mod, s in members:
            if s["name"] in existing:
                continue
            mapping = {ds["name"]: s["name"]}
            ok = True
            for o in range(0, ds["size"], 2):
                ra = relocs[dmod].get(ds["addr"] + o)
                rb = relocs[mod].get(s["addr"] + o)
                if not ra:
                    continue
                if not rb:
                    ok = False
                    break
                ta = names.get(target_module(ra[2]) or "", {}).get(ra[1] & ~1)
                tb = names.get(target_module(rb[2]) or "", {}).get(rb[1] & ~1)
                if ta is None or tb is None:
                    ok = False
                    break
                if mapping.get(ta, tb) != tb:
                    ok = False   # one donor symbol maps to two different targets
                    break
                mapping[ta] = tb
            if not ok:
                continue
            new = text
            # two-phase replace to avoid chains (a->b, b->c)
            for i, k in enumerate(mapping):
                new = re.sub(rf"\b{re.escape(k)}\b", f"@@{i}@@", new)
            for i, v in enumerate(mapping.values()):
                new = new.replace(f"@@{i}@@", v)
            dst = out_dir / f"{s['name']}.c"
            dst.write_text(new)
            r = subprocess.run([sys.executable, str(ROOT / "tools/matching/match.py"), str(dst)],
                               cwd=ROOT, capture_output=True, text=True)
            if r.returncode == 0:
                made += 1
                existing.add(s["name"])
            else:
                dst.unlink()
    print(f"cloned {made} verified functions into src/{args.dir}/")
    return 0


if __name__ == "__main__":
    sys.exit(main())
