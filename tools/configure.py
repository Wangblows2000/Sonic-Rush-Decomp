#!/usr/bin/env python3
"""Generate build.ninja for a Sonic Rush version.

Pipeline (dsd-based, same overall shape as zeldaret/ph):

  baserom --extract--> extract/<ver>/      (dsd rom extract + path-order fix)
  config/<ver> --delink--> build/<ver>/delinks/*.o   (original bytes as ELF)
  src/**.c,cpp --mwccarm--> build/<ver>/src/**.o     (decompiled code)
  dsd lcf --> arm9.lcf + objects.txt;  mwldarm --> build/<ver>/arm9.o
  dsd rom config --> rom_config.yaml;  dsd rom build --> build/rush_<ver>.nds
  fix_header.py; sha1 check against config/versions.yaml

Targets that do not need the Metrowerks toolchain:
  ninja extract | ninja delink | ninja verify | ninja objdiff
The default target (`ninja`) builds and SHA-1-checks the ROM and requires
tools/mwccarm/<version>/ plus wibo (Linux) or Windows.

Usage: python tools/configure.py [eur] [--wibo PATH] [--compiler DIR]
"""

from __future__ import annotations

import argparse
import fnmatch
import json
import shutil
import subprocess
import sys
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import ninja_syntax  # noqa: E402


def find(name: str, explicit: str | None = None) -> str:
    if explicit:
        return explicit
    for c in (ROOT / name, ROOT / "tools" / "bin" / name):
        if c.exists():
            return str(c)
    return shutil.which(name) or name


def compiler_for(path: str, cfg: dict) -> dict:
    for rule in cfg.get("rules", []):
        if fnmatch.fnmatch(path, rule["glob"]):
            return {**cfg["default"], **rule}
    return cfg["default"]


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("version", nargs="?", default="eur")
    ap.add_argument("--wibo", help="wibo or wine executable (Linux/macOS)")
    ap.add_argument("--compiler", type=Path, default=ROOT / "tools" / "mwccarm",
                    help="directory containing <version>/mwccarm.exe")
    ap.add_argument("--dsd")
    ap.add_argument("--objdiff")
    args = ap.parse_args()

    ver = args.version
    versions = yaml.safe_load((ROOT / "config/versions.yaml").read_text())
    if ver not in versions:
        print(f"unknown version {ver}", file=sys.stderr)
        return 1
    v = versions[ver]
    cc_cfg = yaml.safe_load((ROOT / f"config/{ver}/compilers.yaml").read_text())
    dsd = find("dsd", args.dsd)
    objdiff = find("objdiff-cli", args.objdiff)
    wine = "" if sys.platform == "win32" else find("wibo", args.wibo)
    py = sys.executable
    cfg = f"config/{ver}/arm9/config.yaml"
    b = f"build/{ver}"
    ex = f"extract/{ver}"

    have_cc = (args.compiler / cc_cfg["default"]["version"] / "mwccarm.exe").exists()
    if not have_cc:
        print(f"note: {args.compiler}/{cc_cfg['default']['version']}/mwccarm.exe not found; "
              "only extract/delink/verify/objdiff targets will work", file=sys.stderr)

    # Translation units known to dsd (from delinks files)
    try:
        dj = json.loads(subprocess.run([dsd, "json", "delinks", "-c", cfg],
                                       capture_output=True, text=True, check=True).stdout)
    except (OSError, subprocess.CalledProcessError, json.JSONDecodeError):
        dj = None

    # Only translation units registered in a delinks.txt are built; other files
    # under src/ are work in progress and must not affect the ROM.
    registered = set()
    import re as _re0
    for dl in (ROOT / f"config/{ver}").rglob("delinks.txt"):
        # prune TUs whose source vanished (agents may delete their own files)
        t0 = dl.read_text()
        t1 = t0
        for f in _re0.findall(r"^(src/\S+\.(?:c|s)):$", t0, _re0.M):
            if not (ROOT / f).exists():
                t1 = _re0.sub(rf"\n*^{_re0.escape(f)}:\n(?:    .*\n?)*", "\n", t1, flags=_re0.M)
                print(f"pruned missing TU {f}", file=sys.stderr)
        if t1 != t0:
            dl.write_text(t1.rstrip("\n") + "\n")
    for dl in (ROOT / f"config/{ver}").rglob("delinks.txt"):
        for line in dl.read_text().splitlines():
            if line and not line[0].isspace() and line.endswith(":"):
                registered.add(line[:-1])
    sources = sorted(p for p in registered if (ROOT / p).suffix in (".c", ".cpp", ".s"))

    import re as _re
    inc_re = _re.compile(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]', _re.M)
    inc_dirs = [ROOT / "include", ROOT / "libs" / "include"]
    _cache: dict[Path, set[str]] = {}

    def deps_of(path: Path, seen: set | None = None) -> set[str]:
        """Headers a source includes, recursively (only those inside this repo)."""
        if path in _cache:
            return _cache[path]
        seen = seen if seen is not None else set()
        out: set[str] = set()
        try:
            text = path.read_text(errors="replace")
        except OSError:
            return out
        for name in inc_re.findall(text):
            for base in [path.parent, *inc_dirs]:
                h = (base / name)
                if h.is_file():
                    rel = h.resolve().relative_to(ROOT).as_posix()
                    if rel not in seen:
                        seen.add(rel)
                        out.add(rel)
                        out |= deps_of(h.resolve(), seen)
                    break
        _cache[path] = out
        return out

    with (ROOT / "build.ninja").open("w") as fh:
        n = ninja_syntax.Writer(fh, width=120)
        n.comment(f"Generated by tools/configure.py for {ver}. Do not edit.")
        n.variable("ninja_required_version", "1.10")
        n.newline()
        n.rule("extract", f"{py} tools/project.py extract {ver}", description="EXTRACT $in")
        n.rule("delink", f"{dsd} delink --config-path $config && touch $out", description="DELINK")
        n.rule("lcf", f"{dsd} lcf -c $config", description="LCF")
        n.rule("relink_check", f"{py} tools/matching/gnu_relink.py --version {ver} --json $out",
               description="RELINK-CHECK (GNU ld)")
        n.rule("objdiff_cfg", f"{dsd} objdiff --config-path $config --custom-make ninja",
               description="OBJDIFF config")
        n.rule("objdiff_report", f"{objdiff} report generate -o $out", description="OBJDIFF report")
        # mwcc 1.2 writes -MD output to the cwd; use all headers as a coarse dependency instead.
        n.rule("mwcc", f'{wine} "$cc" $cflags $in -o $out', description="MWCC $in")
        n.rule("mwld", f'{wine} "$ld" $ldflags @$objects $lcf -o $out', description="MWLD $out")
        # Assembly TUs (.s) that need exact sub-word layout inside one section, e.g. the
        # secure area: GNU as, `.incbin` of the user's extracted binary for bytes that
        # are not ours to commit (see src/secure/secure_area.s).
        gas = find("arm-none-eabi-as")
        n.rule("gas", f"{gas} -mcpu=arm946e-s -I . $in -o $out", description="GAS $in")
        n.rule("rom_config", f"{dsd} rom config --elf $in --config $config", description="ROM-CONFIG")
        n.rule("rom_build", f"{dsd} rom build --config $in --rom $out && "
                            f"{py} tools/extraction/fix_header.py {ver} $out",
               description="ROM $out")
        n.rule("sha1", f"{py} tools/utilities/sha1check.py $in {v['sha1']} && touch $out",
               description="SHA1 $in")
        n.newline()

        n.build(f"{ex}/config.yaml", "extract", v["baserom"])
        n.build("extract", "phony", f"{ex}/config.yaml")
        cfg_files = sorted(p.relative_to(ROOT).as_posix()
                           for p in (ROOT / f"config/{ver}/arm9").rglob("*")
                           if p.suffix in (".txt", ".yaml"))
        n.build(f"{b}/delinks.stamp", "delink", [f"{ex}/config.yaml"],
                implicit=cfg_files, variables={"config": cfg})
        n.build("delink", "phony", f"{b}/delinks.stamp")
        n.build(f"{b}/reports/gnu_relink.json", "relink_check", f"{b}/delinks.stamp",
                implicit=["tools/matching/gnu_relink.py"])
        n.build("verify", "phony", f"{b}/reports/gnu_relink.json")
        n.build("objdiff.json", "objdiff_cfg", f"{b}/delinks.stamp", variables={"config": cfg})
        n.build("objdiff", "phony", "objdiff.json")

        objs = []
        for src in sources:
            if src.endswith(".s"):
                o = f"{b}/{Path(src).with_suffix('.o').as_posix()}"
                objs.append(o)
                n.build(o, "gas", src, implicit=[f"{ex}/config.yaml"])
                continue
            c = compiler_for(src, cc_cfg)
            flags = " ".join(c["flags"].split())
            head = (ROOT / src).read_text(errors="replace").splitlines()[:3]
            for line in head:  # per-file override: `// flags: ...` (must still be verified by match.py)
                if line.startswith("// flags:"):
                    flags = line[len("// flags:"):].strip()
            if "-nolink" not in flags:
                flags += " -nolink"
            if "-Cpp_exceptions" not in flags:  # .exceptix tables do not exist in the original ROM
                flags += " -Cpp_exceptions off"
            o = f"{b}/{Path(src).with_suffix('.o').as_posix()}"
            objs.append(o)
            n.build(o, "mwcc", src, implicit=sorted(deps_of(ROOT / src)), variables={
                "cc": f"{args.compiler.as_posix()}/{c['version']}/mwccarm.exe",
                "cflags": flags + " -i include -i libs/include"})
        lcf = dj["arm9_lcf_file"] if dj else f"{b}/arm9.lcf"
        objects_txt = dj["arm9_objects_file"] if dj else f"{b}/objects.txt"
        n.build([lcf, objects_txt], "lcf", [f"{b}/delinks.stamp"], variables={"config": cfg})
        ld = cc_cfg["linker"]
        n.build(f"{b}/arm9.o", "mwld", [lcf, objects_txt], implicit=objs, variables={
            "ld": f"{args.compiler.as_posix()}/{ld['version']}/mwldarm.exe",
            "ldflags": ld["flags"] + " -m Entry", "objects": objects_txt, "lcf": lcf})
        n.build(f"{b}/build/rom_config.yaml", "rom_config", f"{b}/arm9.o",
                variables={"config": cfg})
        rom = f"{b}/rush_{ver}.nds"
        n.build(rom, "rom_build", f"{b}/build/rom_config.yaml")
        n.build(f"{b}/rush_{ver}.sha1.ok", "sha1", rom)
        n.build(f"{b}/report.json", "objdiff_report", "objdiff.json", implicit=objs)
        n.build("rom", "phony", f"{b}/rush_{ver}.sha1.ok")
        n.build("report", "phony", f"{b}/report.json")
        n.default("rom" if have_cc else "verify")
    print(f"wrote build.ninja ({len(sources)} source files; default target: "
          f"{'rom' if have_cc else 'verify'})")
    return 0


if __name__ == "__main__":
    sys.exit(main())
