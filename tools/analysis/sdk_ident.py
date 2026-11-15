#!/usr/bin/env python3
"""Identify library functions by compiling a reference decomp's C sources with
our compiler and matching the resulting functions against Sonic Rush.

Reference sources are compiled OUT OF TREE (nothing is copied into this
repository). A Rush function is reported when its masked bytes (relocated
fields masked on both sides) and size equal a reference function's.

    python tools/analysis/sdk_ident.py --ref ../refs/pokediamond/arm9 \
        --src lib/NitroSDK/src --include include-mw lib/NitroSDK/include ../include \
        --flags "-O4,p ..." [--cc 1.2/sp3]

Writes build/<ver>/reports/sdk_ident.csv: rush_address,module,size,ref_name,ref_file,unique
`unique` = the byte pattern occurs once among Rush functions (ambiguous
patterns, e.g. empty stubs, are reported but must not be used for naming).
"""

from __future__ import annotations

import argparse
import csv
import shutil
import subprocess
import sys
import tempfile
from collections import defaultdict
from pathlib import Path

from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "matching"))
from project import modules, parse_symbols, reports_dir  # noqa: E402
from compiler_sweep import extract_symbol, module_bytes  # noqa: E402


def masked(data: bytes, mask: bytes) -> bytes:
    return bytes(a & b for a, b in zip(data, mask))


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--ref", type=Path, required=True, help="reference project root (cwd for compiles)")
    ap.add_argument("--src", nargs="+", required=True, help="source dirs relative to --ref")
    ap.add_argument("--include", nargs="*", default=[], help="include dirs relative to --ref; items starting with - are passed verbatim")
    ap.add_argument("--define", nargs="*", default=[])
    ap.add_argument("--flags", default="-O4,p -proc arm946e -interworking -enum int -gccext,on "
                                        "-fp soft -lang c99 -inline on,noauto -gccinc -char signed")
    ap.add_argument("--cc", default="1.2/sp3")
    ap.add_argument("--version", default="eur")
    ap.add_argument("--label", default="ref")
    args = ap.parse_args()

    exe = ROOT / "tools" / "mwccarm" / args.cc / "mwccarm.exe"
    wibo = shutil.which("wibo") or str(ROOT / "tools/bin/wibo")
    mods = module_bytes(args.version)

    # Index Rush functions by (size, first 4 masked-agnostic bytes) lazily: we
    # compare masked bytes using the reference mask, so bucket by size only.
    by_size: dict[int, list[tuple[str, int, bytes]]] = defaultdict(list)
    for m in modules(args.version):
        base, blob = mods[m["name"]]
        for s in parse_symbols(m["symbols_path"]):
            if s["kind"] == "function" and s["size"]:
                by_size[s["size"]].append((m["name"], s["addr"],
                                           blob[s["addr"] - base:s["addr"] - base + s["size"]]))

    files = sorted(p for d in args.src for p in (args.ref / d).rglob("*.c"))
    inc = []
    for i in args.include:
        if i.startswith("-"):          # verbatim flag, e.g. -I. or -Itools/cw/include/MSL_C
            inc.append(i)
        else:
            inc += ["-ir" if Path(i).name == "include-mw" else "-i", i]
    rows, fails, nfuncs = [], [], 0
    with tempfile.TemporaryDirectory() as tmp:
        for f in files:
            obj = Path(tmp) / (f.stem + ".o")
            cmd = [wibo, str(exe), *args.flags.split(), *inc,
                   *[f"-D{d}" for d in args.define], "-nolink", "-c",
                   str(f.relative_to(args.ref)), "-o", str(obj)]
            r = subprocess.run(cmd, cwd=args.ref, capture_output=True, text=True)
            if r.returncode or not obj.exists():
                fails.append(f.relative_to(args.ref).as_posix())
                continue
            with obj.open("rb") as fh:
                names = [s.name for s in ELFFile(fh).get_section_by_name(".symtab").iter_symbols()
                         if s["st_info"]["type"] == "STT_FUNC" and s["st_shndx"] != "SHN_UNDEF"
                         and not s.name.startswith("$")]
            for name in names:
                got = extract_symbol(obj, name)
                if not got:
                    continue
                data, mask = got
                nfuncs += 1
                key = masked(data, mask)
                hits = [(mod, addr) for mod, addr, want in by_size.get(len(data), [])
                        if masked(want, mask) == key]
                for mod, addr in hits:
                    rows.append({"rush_address": f"0x{addr:08x}", "module": mod, "size": len(data),
                                 "ref_name": name, "ref_file": f.relative_to(args.ref).as_posix(),
                                 "unique": len(hits) == 1})
    # a Rush address claimed by several reference names is ambiguous too
    claims = defaultdict(set)
    for r in rows:
        claims[r["rush_address"]].add(r["ref_name"])
    for r in rows:
        if len(claims[r["rush_address"]]) > 1:
            r["unique"] = False
    out = reports_dir(args.version) / f"sdk_ident_{args.label}.csv"
    with out.open("w", newline="") as fh:
        w = csv.DictWriter(fh, fieldnames=["rush_address", "module", "size", "ref_name", "ref_file", "unique"])
        w.writeheader()
        w.writerows(sorted(rows, key=lambda r: r["rush_address"]))
    uniq = sum(1 for r in rows if r["unique"])
    print(f"{len(files)} files ({len(fails)} failed to compile), {nfuncs} reference functions; "
          f"{len(rows)} hits, {uniq} unique -> {out.relative_to(ROOT)}")
    if fails:
        print("compile failures:", ", ".join(fails[:15]), "..." if len(fails) > 15 else "")
    return 0


if __name__ == "__main__":
    sys.exit(main())
