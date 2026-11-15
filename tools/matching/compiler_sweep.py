#!/usr/bin/env python3
"""Compile probe C files with every candidate mwccarm version/flag set and
compare the resulting function bytes with the original ROM.

Probe format (tools/matching/probes/*.c), first lines:
    // target: <module> <hex address> <symbol-in-probe>
    // mode: arm|thumb            (optional, default arm)
Bytes covered by relocations (BL offsets, literal-pool addresses) are masked
on both sides; everything else must be identical, including length.

Usage:
    python tools/matching/compiler_sweep.py [--versions 1.2/sp3,...] [--flags ...] [probes...]
Prints a match matrix and writes build/eur/reports/compiler_sweep.json.
"""

from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from project import modules, parse_symbols  # noqa: E402

DEFAULT_FLAGS = [
    "-O4,p -proc arm946e -interworking -enum int -char signed -gccext,on -fp soft -lang c99",
    "-O4,s -proc arm946e -interworking -enum int -char signed -gccext,on -fp soft -lang c99",
]


def module_bytes(ver: str) -> dict[str, tuple[int, bytes]]:
    import yaml
    ex = ROOT / "extract" / ver
    rc = yaml.safe_load((ex / "config.yaml").read_text())
    out = {"main": (0x02000000, (ex / rc["arm9_bin"]).read_bytes())}
    for key, base in (("itcm", None), ("dtcm", None)):
        y = yaml.safe_load((ex / rc[key]["config"]).read_text())
        out[key] = (y["base_address"], (ex / rc[key]["bin"]).read_bytes())
    ovs = yaml.safe_load((ex / rc["arm9_overlays"]).read_text())
    for o in ovs["overlays"]:
        out[f"ov{o['id']:03d}"] = (o["base_address"],
                                    (ex / "arm9_overlays" / o["file_name"]).read_bytes())
    return out


def function_size(ver: str, mod: str, addr: int) -> int | None:
    for m in modules(ver):
        if m["name"] == mod:
            for s in parse_symbols(m["symbols_path"]):
                if s["kind"] == "function" and s["addr"] == addr:
                    return s["size"]
    return None


def extract_symbol(obj: Path, name: str) -> tuple[bytes, bytearray] | None:
    with obj.open("rb") as fh:
        e = ELFFile(fh)
        st = e.get_section_by_name(".symtab")
        sym = next((s for s in st.iter_symbols() if s.name == name), None)
        if sym is None or not isinstance(sym["st_shndx"], int):
            return None
        sec = e.get_section(sym["st_shndx"])
        start = sym["st_value"] & ~1
        size = sym["st_size"] or (sec["sh_size"] - start)
        data = sec.data()[start:start + size]
        mask = bytearray(b"\xff" * len(data))
        for rs in e.iter_sections():
            if not isinstance(rs, RelocationSection) or rs["sh_info"] != sym["st_shndx"]:
                continue
            for r in rs.iter_relocations():
                o = r["r_offset"] - start
                if not 0 <= o < len(data):
                    continue
                t = r["r_info_type"]
                if t in (1, 28, 29):            # ARM BL/B: imm24
                    mask[o:o + 3] = b"\0\0\0"
                    if t in (1, 28):            # linker may turn BL into BLX imm (cond/H bits)
                        mask[o + 3] &= 0xEE
                elif t == 10:                   # Thumb BL pair: imm11 x2; linker may turn BL into BLX
                    mask[o] = 0; mask[o + 1] &= 0xF8; mask[o + 2] = 0; mask[o + 3] &= 0xE8
                else:
                    mask[o:o + 4] = b"\0\0\0\0"
        return bytes(data), mask


def run_compiler(cc: Path, flags: str, src: Path, out: Path, wibo: str) -> str | None:
    cmd = ([wibo] if os.name != "nt" else []) + [str(cc), *flags.split(), "-nolink", "-c",
                                                 str(src), "-o", str(out)]
    res = subprocess.run(cmd, capture_output=True, text=True)
    if res.returncode != 0 or not out.exists():
        return (res.stdout + res.stderr).strip()[-300:] or "compile failed"
    return None


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("probes", nargs="*", type=Path)
    ap.add_argument("--version", default="eur")
    ap.add_argument("--compilers", type=Path, default=ROOT / "tools" / "mwccarm")
    ap.add_argument("--versions", help="comma list, default: every dir with mwccarm.exe")
    ap.add_argument("--flags", action="append", help="flag set (repeatable)")
    ap.add_argument("--wibo", default=shutil.which("wibo") or str(ROOT / "tools/bin/wibo"))
    ap.add_argument("--show-bytes", action="store_true")
    args = ap.parse_args()

    probes = args.probes or sorted((ROOT / "tools/matching/probes").glob("*.c"))
    if args.versions:
        versions = args.versions.split(",")
    else:
        versions = sorted(str(p.parent.relative_to(args.compilers))
                          for p in args.compilers.rglob("mwccarm.exe"))
    flag_sets = args.flags or DEFAULT_FLAGS
    mods = module_bytes(args.version)

    results = []
    with tempfile.TemporaryDirectory() as tmp:
        for probe in probes:
            head = probe.read_text().splitlines()[:3]
            m = re.match(r"//\s*target:\s*(\S+)\s+(0x[0-9a-fA-F]+)\s+(\S+)", head[0])
            if not m:
                print(f"skip {probe.name}: no target header")
                continue
            mod, addr, sym = m[1], int(m[2], 16), m[3]
            size = function_size(args.version, mod, addr)
            base, blob = mods[mod]
            want = blob[addr - base: addr - base + size]
            row = {"probe": probe.name, "target": f"{mod}:0x{addr:08x}", "size": size, "results": {}}
            for v in versions:
                for fi, fl in enumerate(flag_sets):
                    obj = Path(tmp) / f"{probe.stem}.o"
                    obj.unlink(missing_ok=True)
                    err = run_compiler(args.compilers / v / "mwccarm.exe", fl, probe, obj, args.wibo)
                    key = f"{v}#{fi}"
                    if err:
                        row["results"][key] = {"status": "error", "detail": err}
                        continue
                    got = extract_symbol(obj, sym)
                    if got is None:
                        row["results"][key] = {"status": "nosym"}
                        continue
                    data, mask = got
                    diff = sum(1 for i in range(min(len(data), len(want)))
                               if (data[i] & mask[i]) != (want[i] & mask[i]))
                    ok = diff == 0 and len(data) == len(want)
                    row["results"][key] = {"status": "match" if ok else "diff", "diff": diff,
                                           "len": len(data), "want_len": len(want),
                                           **({"hex": data.hex()} if args.show_bytes else {})}
            results.append(row)

    # matrix
    keys = [f"{v}#{i}" for v in versions for i in range(len(flag_sets))]
    print("flag sets:")
    for i, f in enumerate(flag_sets):
        print(f"  #{i}: {f}")
    w = max(len(r["probe"]) for r in results) if results else 10
    print(" " * (w + 2) + " ".join(f"{k:>12s}" for k in keys))
    for r in results:
        cells = []
        for k in keys:
            x = r["results"].get(k, {})
            s = x.get("status")
            cells.append({"match": "MATCH", "error": "err", "nosym": "nosym"}.get(
                s, f"d{x.get('diff')}/{x.get('len')}-{x.get('want_len')}"))
        print(f"{r['probe']:<{w}}  " + " ".join(f"{c:>12s}" for c in cells))
    totals = {k: sum(1 for r in results if r["results"].get(k, {}).get("status") == "match") for k in keys}
    print(f"{'matches':<{w}}  " + " ".join(f"{totals[k]:>12d}" for k in keys))
    out = ROOT / "build" / args.version / "reports" / "compiler_sweep.json"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps({"flag_sets": flag_sets, "versions": versions,
                               "results": results, "totals": totals}, indent=2) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
