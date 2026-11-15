#!/usr/bin/env python3
"""Independent relink check: link dsd's delinked objects with GNU ld and
compare every resulting section with the original module bytes.

Why: `dsd delink` turns each module into relocatable ELF objects whose
relocations come from dsd's analysis (relocs.txt). If a relocation is wrong
(bad target, wrong kind, wrong module), relinking at the original addresses
produces different bytes. This check exercises the same objects the matching
build will use, but with a linker we can always run (binutils-arm-none-eabi),
so delink/analysis regressions are caught even without the Metrowerks linker.

It is NOT a substitute for the real mwldarm link: section ordering, padding
and veneers are decided by mwldarm in the matching build.

Usage: python tools/matching/gnu_relink.py --version eur
Exit status 0 only if every compared section matches byte-for-byte.
"""

from __future__ import annotations

import argparse
import json
import re
import shutil
import struct
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path

import yaml
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[2]
SECTION_RE = re.compile(
    r"^\s+(\.\w+)\s+start:0x([0-9a-f]+)\s+end:0x([0-9a-f]+)\s+kind:(\w+)")
FILE_RE = re.compile(r"^(\S.*):$")


@dataclass
class Section:
    name: str
    start: int
    end: int
    kind: str


@dataclass
class Module:
    name: str
    sections: list[Section]
    files: dict[str, list[Section]]  # delink file name -> its sections
    original: bytes
    base: int


def parse_delinks(path: Path) -> tuple[list[Section], dict[str, list[Section]]]:
    """Module sections, plus per-file (translation unit) sections."""
    module: list[Section] = []
    files: dict[str, list[Section]] = {}
    cur: list[Section] | None = module
    for line in path.read_text().splitlines():
        if not line.strip():
            continue
        m = FILE_RE.match(line)
        if m:
            cur = files.setdefault(m.group(1), [])
            continue
        m = SECTION_RE.match(line)
        if m and cur is not None:
            cur.append(Section(m.group(1), int(m.group(2), 16),
                               int(m.group(3), 16), m.group(4)))
    return module, files


def load_modules(cfg_path: Path) -> list[Module]:
    cfg_dir = cfg_path.parent
    cfg = yaml.safe_load(cfg_path.read_text())
    rom_cfg_path = (cfg_dir / cfg["rom_config"]).resolve()
    rom_cfg = yaml.safe_load(rom_cfg_path.read_text())
    ex = rom_cfg_path.parent
    mods: list[Module] = []

    def add(name: str, entry: dict, bin_path: Path) -> None:
        secs, files = parse_delinks(cfg_dir / entry["delinks"])
        mods.append(Module(name, secs, files, bin_path.read_bytes(),
                           secs[0].start))

    add("main", cfg["main_module"], ex / rom_cfg["arm9_bin"])
    for a in cfg.get("autoloads", []):
        key = {"Itcm": "itcm", "Dtcm": "dtcm"}[a["kind"]]
        add(a["name"], a, ex / rom_cfg[key]["bin"])
    ovl = yaml.safe_load((ex / rom_cfg["arm9_overlays"]).read_text())
    ovl_dir = (ex / rom_cfg["arm9_overlays"]).parent
    by_id = {o["id"]: o for o in ovl["overlays"]}
    for o in cfg.get("overlays", []):
        add(o["name"], o, ovl_dir / by_id[o["id"]]["file_name"])
    return mods


def link_order(cfg_path: Path) -> list[str]:
    """Delinked object paths (relative to repo root) in dsd's link order."""
    dsd = shutil.which("dsd") or str(ROOT / "tools/bin/dsd")
    out = subprocess.run([dsd, "json", "delinks", "-c", str(cfg_path)], cwd=ROOT,
                         capture_output=True, text=True, check=True).stdout
    return [str((ROOT / f["delink_file"]).resolve()) for f in json.loads(out)["files"]]


def write_script(mods: list[Module], order: list[str], delinks_root: Path, norm: Path,
                 out: Path) -> dict[str, list[Path]]:
    """One output section per module section; inputs in dsd link order.

    Gap objects are named `_dsd_gap@<module>_<n>`; translation units belong to
    the module whose delinks.txt lists them. We always link the *delinked*
    TU object (original bytes), so this check is independent of src/.
    """
    lines = ["ENTRY(_start_dummy)", "SECTIONS", "{"]
    used: dict[str, list[Path]] = {m.name: [] for m in mods}
    tu_owner = {Path(f).with_suffix(".o").as_posix(): m.name for m in mods for f in m.files}
    for rel in order:
        p = Path(rel)
        local = norm / p.relative_to(delinks_root)
        name = p.name
        if name.startswith("_dsd_gap@"):
            mod = name[len("_dsd_gap@"):].rsplit("_", 1)[0]
        else:
            mod = tu_owner.get(p.relative_to(delinks_root).as_posix())
        if mod in used:
            used[mod].append(local)
    for mod in mods:
        for s in mod.sections:
            kind = "(NOLOAD)" if s.kind == "bss" else ""
            lines.append(f"  {mod.name}{s.name} 0x{s.start:08x} {kind} : {{")
            for obj in used[mod.name]:
                lines.append(f"    \"{obj.as_posix()}\"({s.name})")
            lines.append("  }")
    lines += ["  /DISCARD/ : { *(*) }", "}", "_start_dummy = 0;"]
    # dsd `link_time_const(...)` relocations (docs/relocs.md in ds-decomp).
    by = {m.name: m for m in mods}
    def sec(mod: str, name: str) -> Section | None:
        m = by.get(mod)
        return next((s for s in m.sections if s.name == name), None) if m else None
    consts = {}
    if (s := sec("main", ".ctor")):
        consts["ARM9_CTOR_START"] = s.start
    if "dtcm" in by:
        consts["__DTCM_LO"] = by["dtcm"].sections[0].start
    if "itcm" in by:
        consts["__ITCM_HI"] = max(s.end for s in by["itcm"].sections)
    consts["__OVERLAY_COUNT"] = sum(1 for m in mods if m.name.startswith("ov"))
    for k, v in consts.items():
        lines.append(f"{k} = 0x{v:08x};")
    out.write_text("\n".join(lines) + "\n")
    return used


def mark_thumb_functions(path: Path) -> int:
    """Set bit 0 of defined STT_FUNC symbols that lie in Thumb code.

    dsd marks Thumb code only with `$t` mapping symbols and writes Thumb
    functions with even values. AAELF says a Thumb function symbol's value
    has bit 0 set; without it GNU ld treats calls to it as ARM and inserts
    interworking veneers. Returns the number of symbols changed.
    """
    data = bytearray(path.read_bytes())
    with path.open("rb") as fh:
        e = ELFFile(fh)
        st = e.get_section_by_name(".symtab")
        off, ent = st["sh_offset"], st["sh_entsize"]
        maps: dict[int, list[tuple[int, str]]] = {}
        syms = list(st.iter_symbols())
        for s in syms:
            if s.name in ("$a", "$t", "$d") or s.name.startswith(("$a.", "$t.", "$d.")):
                maps.setdefault(s["st_shndx"], []).append((s["st_value"], s.name[1]))
    for v in maps.values():
        v.sort()
    changed = 0
    for i, s in enumerate(syms):
        if s["st_info"]["type"] != "STT_FUNC" or not isinstance(s["st_shndx"], int):
            continue
        val = s["st_value"]
        if val & 1:
            continue
        m = maps.get(s["st_shndx"], [])
        mode = None
        for a, k in m:
            if a > val:
                break
            mode = k
        if mode == "t":
            struct.pack_into("<I", data, off + i * ent + 4, val | 1)
            changed += 1
    path.write_bytes(data)
    return changed


R_ARM_PC24, R_ARM_ABS32, R_ARM_CALL, R_ARM_JUMP24 = 1, 2, 28, 29


def modernize_arm_branch_relocs(path: Path) -> int:
    """Retype legacy R_ARM_PC24 relocations on ARM branches.

    dsd emits R_ARM_PC24 for ARM `bl`/`blx`/`b`. GNU ld may not convert an
    R_ARM_PC24 `bl` into `blx` for a Thumb target and wants a `.glue_7` stub
    instead. AAELF's R_ARM_CALL (bl/blx) and R_ARM_JUMP24 (b, conditional bl)
    express the same field and let `--use-blx` reproduce the original `blx`.
    """
    data = bytearray(path.read_bytes())
    changed = 0
    with path.open("rb") as fh:
        e = ELFFile(fh)
        for rs in e.iter_sections():
            if rs["sh_type"] not in ("SHT_REL", "SHT_RELA"):
                continue
            target = e.get_section(rs["sh_info"])
            tdata = target.data()
            ent, base = rs["sh_entsize"], rs["sh_offset"]
            for i in range(rs["sh_size"] // ent):
                o = base + i * ent
                r_off, r_info = struct.unpack_from("<II", data, o)
                if r_info & 0xFF == R_ARM_ABS32 and rs["sh_type"] == "SHT_RELA":
                    # dsd leaves the original word in place for some ABS32
                    # relocations (seen on link_time_const). The RELA addend
                    # is authoritative; GNU ld also adds the in-place value.
                    struct.pack_into("<I", data, target["sh_offset"] + r_off, 0)
                    continue
                if r_info & 0xFF != R_ARM_PC24:
                    continue
                ins = struct.unpack_from("<I", tdata, r_off)[0]
                cond, op = ins >> 28, (ins >> 24) & 0xF
                if cond == 0xF or (cond == 0xE and op == 0xB):
                    new = R_ARM_CALL       # blx imm / unconditional bl
                else:
                    new = R_ARM_JUMP24     # b / conditional branches
                struct.pack_into("<I", data, o + 4, (r_info & ~0xFF) | new)
                changed += 1
    path.write_bytes(data)
    return changed


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--version", default="eur")
    ap.add_argument("--ld", default="arm-none-eabi-ld")
    ap.add_argument("--json", type=Path, help="write a JSON report here")
    args = ap.parse_args()

    if not shutil.which(args.ld):
        print(f"error: {args.ld} not found (install binutils-arm-none-eabi)",
              file=sys.stderr)
        return 2
    cfg_path = ROOT / "config" / args.version / "arm9" / "config.yaml"
    cfg = yaml.safe_load(cfg_path.read_text())
    delinks_dir = (cfg_path.parent / cfg["delinks_path"]).resolve()
    if not delinks_dir.is_dir():
        print(f"error: {delinks_dir} missing; run `dsd delink` first",
              file=sys.stderr)
        return 2

    mods = load_modules(cfg_path)
    work = ROOT / "build" / args.version / "gnu_relink"
    work.mkdir(parents=True, exist_ok=True)
    # dsd 0.12.1 writes some GLOBAL symbols before .symtab's sh_info boundary
    # (first non-local index). GNU ld then treats them as local and cannot
    # resolve undefined ones. objcopy rewrites the table in canonical order
    # without touching section contents. See docs/research/0006-gnu-relink.md.
    norm = work / "objs"
    norm.mkdir(exist_ok=True)
    objcopy = args.ld.replace("-ld", "-objcopy")
    for src in delinks_dir.rglob("*.o"):
        dst = norm / src.relative_to(delinks_dir)
        dst.parent.mkdir(parents=True, exist_ok=True)
        if not dst.exists() or dst.stat().st_mtime < src.stat().st_mtime:
            subprocess.run([objcopy, str(src), str(dst)], check=True)
            mark_thumb_functions(dst)
            modernize_arm_branch_relocs(dst)
    script = work / "relink.ld"
    order = link_order(cfg_path)
    used = write_script(mods, order, delinks_dir.resolve(), norm, script)
    objs = sorted({o for v in used.values() for o in v})
    elf = work / "relink.elf"
    cmd = [args.ld, "--no-check-sections", "--use-blx", "-T", str(script), "-o", str(elf),
           *map(str, objs)]
    res = subprocess.run(cmd, capture_output=True, text=True)
    # GNU ld warns on every Thumb<->ARM BL because the delinked objects carry no
    # interworking attributes; the encoded instructions are what we compare.
    errs = [l for l in res.stderr.splitlines() if "interworking not enabled" not in l]
    if res.returncode != 0:
        print("\n".join(errs[-60:]), file=sys.stderr)
        return 1

    report = {"version": args.version, "modules": []}
    ok = True
    with elf.open("rb") as fh:
        e = ELFFile(fh)
        for mod in mods:
            mrep = {"module": mod.name, "sections": []}
            for s in mod.sections:
                if s.kind == "bss" or s.end == s.start:
                    continue
                sec = e.get_section_by_name(f"{mod.name}{s.name}")
                want = mod.original[s.start - mod.base:s.end - mod.base]
                got = sec.data() if sec is not None else b""
                diffs = sum(1 for a, b in zip(got, want) if a != b) + abs(len(got) - len(want))
                first = next((i for i, (a, b) in enumerate(zip(got, want)) if a != b), None)
                match = diffs == 0
                ok &= match
                mrep["sections"].append({
                    "section": s.name, "start": f"0x{s.start:08x}",
                    "size": s.end - s.start, "got_size": len(got),
                    "match": match, "differing_bytes": diffs,
                    "first_diff": None if first is None else f"0x{s.start + first:08x}",
                })
                status = "OK  " if match else "DIFF"
                extra = "" if match else f"  {diffs} bytes differ, first at {mrep['sections'][-1]['first_diff']}"
                print(f"{status} {mod.name:6s} {s.name:8s} 0x{s.start:08x}-0x{s.end:08x}{extra}")
            report["modules"].append(mrep)
    report["match"] = ok
    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(report, indent=2) + "\n")
    print("RESULT:", "all sections match" if ok else "MISMATCH")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
