#!/usr/bin/env python3
"""Compile one C file and compare every function it defines with the ROM.

    python tools/matching/match.py src/main/func_0201bf1c.c [--cc 1.2/sp3] [--flags "..."]
    python tools/matching/match.py scratch.c --promote   # add delinks TU entry on full match

Functions are looked up by name in config/<ver>/arm9/**/symbols.txt
(e.g. `func_0201bf1c`, `func_ov001_022c5c80`, or an already-renamed symbol).
Relocated fields are masked on both sides. On mismatch, prints an aligned
disassembly diff (capstone). Exit 0 only if every function matches.
"""

from __future__ import annotations

import argparse
import difflib
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

import capstone
import yaml
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "matching"))
from project import modules, parse_symbols  # noqa: E402
from compiler_sweep import extract_symbol, module_bytes  # noqa: E402


def disasm(data: bytes, mask: bytes, addr: int, thumb: bool) -> list[str]:
    md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB if thumb else capstone.CS_MODE_ARM)
    md.skipdata = True
    out = []
    for i in md.disasm(data, addr):
        off = i.address - addr
        m = mask[off:off + i.size]
        txt = f"{i.mnemonic} {i.op_str}"
        if any(b != 0xFF for b in m):
            txt = f"{i.mnemonic} <reloc>"
        out.append(f"{off:04x}: {txt}")
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("source", type=Path)
    ap.add_argument("--version", default="eur")
    ap.add_argument("--cc", help="compiler version dir under tools/mwccarm (default: compilers.yaml)")
    ap.add_argument("--flags")
    ap.add_argument("--promote", action="store_true", help="add a delinks.txt TU entry on full match")
    args = ap.parse_args()

    cc_cfg = yaml.safe_load((ROOT / f"config/{args.version}/compilers.yaml").read_text())
    cc = args.cc or cc_cfg["default"]["version"]
    flags = args.flags or " ".join(cc_cfg["default"]["flags"].split())
    if not args.flags:
        for line in args.source.read_text(errors="replace").splitlines()[:3]:
            if line.startswith("// flags:"):
                flags = line[len("// flags:"):].strip()
    if "-Cpp_exceptions" not in flags:  # .exceptix tables do not exist in the original ROM
        flags += " -Cpp_exceptions off"
    exe = ROOT / "tools" / "mwccarm" / cc / "mwccarm.exe"
    wibo = shutil.which("wibo") or str(ROOT / "tools/bin/wibo")

    syms = {}
    for m in modules(args.version):
        for s in parse_symbols(m["symbols_path"]):
            if s["kind"] == "function":
                syms[s["name"]] = (m, s)
    mods = module_bytes(args.version)

    with tempfile.TemporaryDirectory() as tmp:
        obj = Path(tmp) / "out.o"
        flist = [f for f in flags.split() if f != "-nolink"]
        cmd = [wibo, str(exe), *flist, "-i", str(ROOT / "include"), "-nolink", "-c",
               str(args.source), "-o", str(obj)]
        r = subprocess.run(cmd, capture_output=True, text=True)
        msgs = "\n".join(l for l in (r.stdout + r.stderr).splitlines() if "MWCIncludes" not in l
                         and "Usage Warning" not in l)
        if r.returncode or not obj.exists():
            print(msgs)
            return 2
        with obj.open("rb") as fh:
            e = ELFFile(fh)
            defined = [s.name for s in e.get_section_by_name(".symtab").iter_symbols()
                       if s["st_info"]["type"] == "STT_FUNC" and s["st_shndx"] != "SHN_UNDEF"
                       and not s.name.startswith("$")]
        all_ok, matched = True, []
        for name in defined:
            if name not in syms:
                print(f"?    {name}: not in symbols.txt")
                all_ok = False
                continue
            m, s = syms[name]
            base, blob = mods[m["name"]]
            want = blob[s["addr"] - base:s["addr"] - base + s["size"]]
            data, mask = extract_symbol(obj, name)
            wmask = bytearray(mask[:len(want)]) + bytearray(b"\xff" * max(0, len(want) - len(mask)))
            diff = sum(1 for i in range(min(len(data), len(want)))
                       if (data[i] & mask[i]) != (want[i] & mask[i]))
            ok = diff == 0 and len(data) == len(want)
            all_ok &= ok
            print(f"{'OK  ' if ok else 'DIFF'} {name} ({m['name']} 0x{s['addr']:08x}, "
                  f"{len(data)}/{len(want)} bytes, {diff} differ) [{cc}]")
            if ok:
                matched.append((m, s))
            else:
                thumb = s["mode"] == "thumb"
                a = disasm(want, wmask, s["addr"], thumb)
                b = disasm(data, mask, s["addr"], thumb)
                for line in difflib.unified_diff(a, b, "target", "compiled", lineterm="", n=2):
                    print("   ", line)

    if args.promote and all_ok and matched:
        rel = args.source.resolve().relative_to(ROOT).as_posix()
        for m, s in matched:
            p = m["delinks_path"]
            t = p.read_text()
            if f"{rel}:" in t:
                print(f"already in {p.relative_to(ROOT)}")
                continue
            lo = min(x[1]["addr"] for x in matched if x[0]["name"] == m["name"])
            hi = max(x[1]["addr"] + x[1]["size"] for x in matched if x[0]["name"] == m["name"])
            hi = (hi + 3) & ~3  # LCF uses ALIGNALL(4): TU must end on a word boundary (Thumb padding)
            import re as _re
            taken = [(int(a, 16), int(b, 16)) for a, b in
                     _re.findall(r"^    \.text\s+start:0x([0-9a-f]+) end:0x([0-9a-f]+)\s*$", t, _re.M)]
            if any(a < hi and lo < b for a, b in taken):
                print(f"duplicate: 0x{lo:08x}-0x{hi:08x} already covered by another TU")
                return 3
            p.write_text(t.rstrip("\n") + f"\n\n{rel}:\n    complete\n"
                         f"    .text       start:0x{lo:08x} end:0x{hi:08x}\n")
            print(f"promoted to {p.relative_to(ROOT)}")
            break
    return 0 if all_ok else 1


if __name__ == "__main__":
    sys.exit(main())
