#!/usr/bin/env python3
"""Move translation units from per-agent directories into the subsystem layout.

Layout (PLAN.md section 4):
  src/libs/NitroSDK/<lib>/      NitroSDK 2.1 (main 0x02000800-0x0201b194, plus ITCM SDK code)
  src/libs/NitroSDK/demolib/    NitroSDK wireless demo libs wh/wfs/mbp (0x0203af64-0x0204069c)
  src/libs/NitroSystem/<lib>/   NitroSystem (0x0201b194-0x0202a08c)
  src/libs/MSL/                 MSL C (0x0202a08c-0x0202d5d8)
  src/libs/Runtime/             CW ARM runtime (0x0202d5d8-0x0202fd14)
  src/game/<module>/<Prefix>/   game code, grouped by the symbol's subsystem prefix

src/asm/, src/asm_fallback/ and src/secure/ stay where they are (tools rely on them).
Ranges come from docs/naming-conventions.md section 3.1 and the delinks addresses.
Only paths change: delinks.txt entry order, sections and compiler flags are untouched.

Usage: python3 tools/utilities/reorg_layout.py [--dry]
"""
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CFG = ROOT / "config/eur/arm9"
DELINKS = {
    "main": CFG / "delinks.txt",
    "itcm": CFG / "itcm/delinks.txt",
    "ov000": CFG / "overlays/ov000/delinks.txt",
    "ov001": CFG / "overlays/ov001/delinks.txt",
}
KEEP = ("src/asm/", "src/asm_fallback/", "src/secure/")

SDK = {
    "OS": "os", "OSi": "os", "GX": "gx", "GXi": "gx", "G2": "gx", "G2S": "gx", "G2i": "gx",
    "G3": "gx", "G3X": "gx", "G3i": "gx", "G3BS": "gx", "FX": "fx", "FXi": "fx", "MTX": "fx",
    "VEC": "fx", "CP": "fx", "MI": "mi", "MIi": "mi", "SND": "snd", "SNDi": "snd", "PXI": "pxi",
    "PXIi": "pxi", "FS": "fs", "FSi": "fs", "PM": "spi", "PMi": "spi", "TP": "spi", "TPi": "spi",
    "MIC": "spi", "MICi": "spi", "NVRAM": "spi", "RTC": "rtc", "RTCi": "rtc", "CARD": "card",
    "CARDi": "card", "WM": "wm", "WMi": "wm", "MB": "mb", "MBi": "mb", "WBT": "wbt",
    "CTRDG": "ctrdg", "CTRDGi": "ctrdg", "MATH": "math", "MATHi": "math", "DGT": "math",
    "CHT": "cht", "STD": "std", "EXI": "os", "INIT": "init", "INITi": "init",
}
NNS = (("NNS_Snd", "snd"), ("NNSi_Snd", "snd"), ("NNS_Fnd", "fnd"), ("NNSi_Fnd", "fnd"),
       ("NNS_G3d", "g3d"), ("NNSi_G3d", "g3d"), ("NNS_G2d", "g2d"), ("NNSi_G2d", "g2d"),
       ("NNS_Gfd", "gfd"), ("NNSi_Gfd", "gfd"))


def parse(path):
    """Yield (tu_path, text_start or None) in file order."""
    cur, start = None, None
    for line in path.read_text().splitlines():
        if line.startswith("src/") and line.endswith(":"):
            if cur:
                yield cur, start
            cur, start = line[:-1], None
        m = re.search(r"\.text\s+start:0x([0-9a-fA-F]+)", line)
        if m and cur and start is None:
            start = int(m.group(1), 16)
    if cur:
        yield cur, start


def prefix(name):
    return name.split("_", 1)[0] if "_" in name.strip("_") else ""


def game_group(name, carry):
    p = prefix(name)
    if not p or p == "func" or not p[0].isalpha():
        return carry or "Misc"
    return p


def classify(module, name, addr, carry):
    """Return (dest_dir, new_carry)."""
    if module in ("main", "itcm"):
        if module == "itcm" or addr is None:
            p = prefix(name)
            if p in SDK:
                return f"src/libs/NitroSDK/{SDK[p]}", SDK[p]
            g = game_group(name, carry if carry and not carry.islower() else None)
            return f"src/game/{module}/{g}", g
        if 0x02000800 <= addr < 0x02000b24:
            return "src/libs/NitroSDK/init", carry
        if 0x02000b24 <= addr < 0x02000cb4:
            return "src/game/main/Main", carry
        if 0x02000cb4 <= addr < 0x0201b194:
            p = prefix(name)
            lib = SDK.get(p) or (carry if carry in SDK.values() else "os")
            return f"src/libs/NitroSDK/{lib}", lib
        if 0x0201b194 <= addr < 0x0202a08c:
            for pre, lib in NNS:
                if name.startswith(pre):
                    return f"src/libs/NitroSystem/{lib}", lib
            lib = carry if carry in ("snd", "fnd", "g3d", "g2d", "gfd") else "snd"
            return f"src/libs/NitroSystem/{lib}", lib
        if 0x0202a08c <= addr < 0x0202d5d8:
            return "src/libs/MSL", carry
        if 0x0202d5d8 <= addr < 0x0202fd14:
            return "src/libs/Runtime", carry
        if 0x0203af64 <= addr < 0x0204069c:
            return "src/libs/NitroSDK/demolib", carry
    g = game_group(name, carry if carry and not carry.islower() else None)
    return f"src/game/{module}/{g}", g


def main():
    dry = "--dry" in sys.argv
    moves = {}
    for module, path in DELINKS.items():
        entries = list(parse(path))
        order = sorted(range(len(entries)), key=lambda i: (entries[i][1] is None, entries[i][1] or 0))
        carry = None
        for i in order:
            tu, addr = entries[i]
            if tu.startswith(KEEP):
                continue
            name = Path(tu).stem
            dest, carry = classify(module, name, addr, carry)
            moves[tu] = f"{dest}/{Path(tu).name}"
    targets = {}
    for old, new in moves.items():
        if new.lower() in targets:
            sys.exit(f"collision: {old} and {targets[new.lower()]} -> {new}")
        targets[new.lower()] = old
    dirs = {}
    for new in moves.values():
        d = str(Path(new).parent)
        if d.lower() in dirs and dirs[d.lower()] != d:
            sys.exit(f"case-only directory collision: {d} vs {dirs[d.lower()]}")
        dirs[d.lower()] = d
    changed = {o: n for o, n in moves.items() if o != n}
    print(f"{len(moves)} TUs, {len(changed)} to move, {len(dirs)} directories")
    if dry:
        for o, n in list(changed.items())[:40]:
            print(f"  {o} -> {n}")
        return
    for old, new in changed.items():
        (ROOT / new).parent.mkdir(parents=True, exist_ok=True)
        subprocess.run(["git", "mv", old, new], cwd=ROOT, check=True)
    for path in DELINKS.values():
        text = path.read_text().splitlines(keepends=True)
        out = []
        for line in text:
            s = line.rstrip("\n")
            if s.startswith("src/") and s.endswith(":") and s[:-1] in changed:
                line = changed[s[:-1]] + ":\n"
            out.append(line)
        path.write_text("".join(out))
    print("done")


if __name__ == "__main__":
    main()
