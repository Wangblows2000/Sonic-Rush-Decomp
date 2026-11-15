#!/usr/bin/env python3
"""Call graph from dsd relocs.txt, and a ranked list of first-match candidates.

Edges come from call/branch relocations (arm_call, thumb_call, arm_call_thumb,
thumb_call_arm, arm_branch) whose source lies inside a known function.
Function-pointer loads (`load` to a function start) are counted separately.

Writes build/<ver>/reports/callgraph.csv and candidates.csv.
Candidates = small leaf (or near-leaf) functions, scored to favour code that
is cheap to match and widely used: size <= --max-size, few callees, many
callers. This ranks work; it does not claim anything about names.
"""

from __future__ import annotations

import argparse
import bisect
import csv
import re
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from project import modules, parse_symbols, reports_dir  # noqa: E402

REL_RE = re.compile(r"^from:0x([0-9a-f]+) kind:(\S+) to:0x([0-9a-f]+)(?: add:\S+)? module:(\S+)")
CALL_KINDS = {"arm_call", "thumb_call", "arm_call_thumb", "thumb_call_arm", "arm_branch"}


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("version", nargs="?", default="eur")
    ap.add_argument("--max-size", type=lambda x: int(x, 0), default=0x40)
    ap.add_argument("--top", type=int, default=25)
    args = ap.parse_args()

    funcs: dict[tuple[str, int], dict] = {}
    starts: dict[str, list[int]] = {}
    mods = modules(args.version)
    for m in mods:
        lst = []
        for s in parse_symbols(m["symbols_path"]):
            if s["kind"] != "function":
                continue
            key = (m["name"], s["addr"])
            funcs[key] = {"module": m["name"], "addr": s["addr"], "name": s["name"],
                          "size": s["size"] or 0, "mode": s["mode"],
                          "callers": set(), "callees": set(), "ptr_refs": 0}
            lst.append(s["addr"])
        starts[m["name"]] = sorted(lst)

    def owner(mod: str, addr: int):
        lst = starts[mod]
        i = bisect.bisect_right(lst, addr) - 1
        if i < 0:
            return None
        f = funcs[(mod, lst[i])]
        return f if addr < f["addr"] + max(f["size"], 1) else None

    def target_module(src_mod: str, spec: str) -> str | None:
        if spec == "main":
            return "main"
        if spec in ("itcm", "dtcm"):
            return spec
        m = re.match(r"overlays?\((\d+)", spec)
        if m:
            return f"ov{int(m.group(1)):03d}"
        return None

    edges = 0
    for m in mods:
        relp = m["symbols_path"].parent / Path(m["relocations"]).name
        if not relp.exists():
            continue
        for line in relp.read_text().splitlines():
            r = REL_RE.match(line)
            if not r:
                continue
            frm, kind, to, spec = int(r[1], 16), r[2], int(r[3], 16), r[4]
            tmod = target_module(m["name"], spec)
            if tmod is None:
                continue
            callee = funcs.get((tmod, to & ~1))
            if callee is None:
                continue
            if kind in CALL_KINDS:
                caller = owner(m["name"], frm)
                if caller is None or caller is callee:
                    continue
                caller["callees"].add((tmod, callee["addr"]))
                callee["callers"].add((caller["module"], caller["addr"]))
                edges += 1
            elif kind == "load":
                callee["ptr_refs"] += 1

    out = reports_dir(args.version)
    with (out / "callgraph.csv").open("w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(["module", "address", "name", "mode", "size", "callers", "callees", "ptr_refs"])
        for f in sorted(funcs.values(), key=lambda f: (f["module"], f["addr"])):
            w.writerow([f["module"], f"0x{f['addr']:08x}", f["name"], f["mode"], f["size"],
                        len(f["callers"]), len(f["callees"]), f["ptr_refs"]])

    cands = [f for f in funcs.values() if 4 < f["size"] <= args.max_size and len(f["callees"]) <= 1]
    for f in cands:
        f["score"] = (len(f["callers"]) + f["ptr_refs"]) * 10 - f["size"] // 4 - 5 * len(f["callees"])
    cands.sort(key=lambda f: (-f["score"], f["module"], f["addr"]))
    with (out / "candidates.csv").open("w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(["score", "module", "address", "name", "mode", "size", "callers", "callees", "ptr_refs"])
        for f in cands:
            w.writerow([f["score"], f["module"], f"0x{f['addr']:08x}", f["name"], f["mode"],
                        f["size"], len(f["callers"]), len(f["callees"]), f["ptr_refs"]])
    leaves = sum(1 for f in funcs.values() if not f["callees"])
    print(f"{len(funcs)} functions, {edges} call edges, {leaves} leaves, "
          f"{len(cands)} candidates (size <= {args.max_size:#x}, <= 1 callee)")
    print(f"{'score':>5} {'module':6} {'address':10} {'mode':5} {'size':>5} {'callers':>7} {'ptrs':>4}  name")
    for f in cands[:args.top]:
        print(f"{f['score']:5d} {f['module']:6} 0x{f['addr']:08x} {f['mode'] or '':5} {f['size']:5d} "
              f"{len(f['callers']):7d} {f['ptr_refs']:4d}  {f['name']}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
