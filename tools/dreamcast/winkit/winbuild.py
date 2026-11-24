#!/usr/bin/env python3
"""Sonic Rush DC — make the Dreamcast disc from the owner's Sonic Rush (Europe) ROM, on Windows.

Run by the kit's GUI (SonicRushDC.exe) with the bundled Python:
    python.exe -u winbuild.py --rom <rom.nds> --out <folder>
It also runs on Linux for testing (tools from PATH / the repository instead of the kit's bin\\).

Everything on the disc is converted here from the owner's ROM; the kit itself contains no game data.
The finished disc contains the game's data: it must not be redistributed.

Progress lines for the GUI (everything else is log text):
    @@STEP <n> <total> <text>     a new step starts
    @@PCT <0..100>                overall progress
    @@DONE <path of the .cdi>
    @@ERROR <message for the player>
"""
import argparse
import hashlib
import os
import shutil
import subprocess
import sys
import time
import traceback
from pathlib import Path

APP = Path(__file__).resolve().parent            # the kit's app\ folder: a copy of the repository layout
WIN = os.name == "nt"
EXPECTED_SHA1 = "52e5024efcf7617a820fa23789b37e5168c3c4fc"
STEPS = 5


def say(line):
    print(line, flush=True)


def step(n, text):
    say("@@STEP %d %d %s" % (n, STEPS, text))


def pct(v):
    say("@@PCT %d" % max(0, min(100, int(v))))


class Fail(Exception):
    """an error the player can act on (shown as-is)"""


def tool(name):
    """the kit's bin\\<name>.exe on Windows; on Linux the tool on PATH (testing)"""
    if WIN:
        p = APP / "bin" / (name + ".exe")
        if not p.is_file():
            raise Fail("The kit is incomplete: bin\\%s.exe is missing. Unzip the whole kit again." % name)
        return str(p)
    return shutil.which(name) or name


def run(args, cwd=APP, env_extra=None, quiet=False):
    env = dict(os.environ)
    env.update({"PYTHONUTF8": "1", "PYTHONIOENCODING": "utf-8", "PYTHONDONTWRITEBYTECODE": "1"})
    if WIN:
        env["SRDC_SSEQRENDER"] = str(APP / "bin" / "sseqrender.exe")
        env["PATH"] = str(APP / "bin") + os.pathsep + env.get("PATH", "")
    if env_extra:
        env.update(env_extra)
    say("> " + " ".join(str(a) for a in args))
    p = subprocess.Popen([str(a) for a in args], cwd=str(cwd), env=env, stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT, text=True, encoding="utf-8", errors="replace")
    tail = []
    for line in p.stdout:
        line = line.rstrip()
        tail = (tail + [line])[-15:]
        if not quiet:
            say("  " + line)
    if p.wait() != 0:
        raise RuntimeError("%s failed (exit %d):\n%s" % (Path(str(args[1] if args[0] == sys.executable else args[0])).name,
                                                         p.returncode, "\n".join(tail)))


def py(script, *args, **kw):
    run([sys.executable, APP / script] + list(args), **kw)


def check_rom(rom):
    rom = Path(rom)
    if not rom.is_file():
        raise Fail("The ROM file was not found:\n%s" % rom)
    size = rom.stat().st_size
    with open(rom, "rb") as f:
        head = f.read(0x200)
    if size < 0x200 or head[0x0C:0x10] not in (b"ASCP", b"ASCE", b"ASCJ"):
        title = head[0:12].rstrip(b"\0").decode("ascii", "replace") if size >= 12 else ""
        raise Fail("This file is not a Sonic Rush ROM%s.\nPick your Sonic Rush (Europe) .nds file." %
                   (" (it says \"%s\")" % title if title.strip() else ""))
    if head[0x0C:0x10] != b"ASCP":
        raise Fail("This is the %s version of Sonic Rush. The disc maker needs the European version "
                   "(game code ASCP)." % {"ASCE": "American", "ASCJ": "Japanese"}[head[0x0C:0x10].decode()])
    h = hashlib.sha1()
    with open(rom, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    sha = h.hexdigest()
    if sha != EXPECTED_SHA1:
        say("warning: ROM SHA-1 %s is not the tested dump %s; trying anyway" % (sha, EXPECTED_SHA1))
    return sha


def extract(rom, sha):
    ext = APP / "extract" / "eur"
    stamp = APP / "extract" / "rom.sha1"
    if ext.is_dir() and (ext / "files").is_dir() and stamp.is_file() and stamp.read_text().strip() == sha:
        say("ROM already extracted")
        return
    if ext.exists():
        shutil.rmtree(ext)
    (APP / "extract").mkdir(exist_ok=True)
    shutil.copyfile(rom, APP / "extract" / "baserom_rush_eur.nds")
    run([tool("dsd"), "rom", "extract", "--rom", APP / "extract" / "baserom_rush_eur.nds", "--output-path", ext])
    py("tools/extraction/fix_path_order.py", APP / "extract" / "baserom_rush_eur.nds", ext / "path_order.txt")
    stamp.write_text(sha)


# converters in make_disc.sh order: (label, script, args, weight in seconds of a typical run)
def converters():
    b = APP / "build" / "dreamcast"
    return [
        ("levels", "tools/dreamcast/stages/stage2dc.py", ["--all", "--no-preview"], 25),
        ("sprites", "tools/dreamcast/sprites/bac2dc.py", ["--no-preview"], 45),
        ("3D models", "tools/dreamcast/models/nsbmd2dc.py", ["batch"], 25),
        ("music and sounds", "tools/dreamcast/audio/sdat2dc.py", ["--wav2adpcm", tool("wav2adpcm")], 90),
        ("sound banks", "tools/dreamcast/audio/sfxbank.py", [], 8),
        ("music streams", "tools/dreamcast/audio/musictab.py", [], 25),
        ("player", "dreamcast/game/player/tools/extract_physics.py", [], 2),
        ("objects", "dreamcast/game/objects/tools/objdata.py", ["--out", b / "objects"], 6),
        ("HUD", "dreamcast/game/ui/tools/uidata.py", ["--out", b / "ui"], 3),
        ("bosses", "dreamcast/game/boss/tools/bossdata.py", ["--out", b / "boss"], 6),
        ("Extra boss", "dreamcast/game/boss/tools/bossexdata.py", ["--out", b / "boss" / "extra"], 4),
        ("HUD backgrounds", "dreamcast/game/ui/tools/uibg.py", ["--out", b / "ui"], 4),
        ("menus", "dreamcast/game/menus/tools/menudata.py", ["--out", b / "menus"], 10),
        ("special stages", "dreamcast/game/special/tools/spdata.py", [], 12),
        ("cutscenes", "dreamcast/game/cutscene/tools/csdata.py", ["--out", b / "cutscene"], 12),
        ("cutscene sounds", "dreamcast/game/cutscene/tools/csbank.py", ["--out", b / "cutscene" / "sound"], 6),
    ]


def convert(sha, lo, hi):
    stamp = APP / "build" / "dreamcast" / "converted.sha1"
    if stamp.is_file() and stamp.read_text().strip() == sha:
        say("assets already converted from this ROM")
        pct(hi)
        return
    if stamp.exists():
        stamp.unlink()
    conv = converters()
    total = sum(c[3] for c in conv)
    done = 0
    for label, script, args, w in conv:
        say("@@SUB converting %s" % label)
        py(script, *args)
        done += w
        pct(lo + (hi - lo) * done / total)
    stamp.write_text(sha)


def make_disc(out_dir):
    work = APP / "build" / "disc"
    pre = APP / "prebuilt"
    py("tools/dreamcast/disc/assemble_disc.py", "--root", APP, "--out", work, "--first-read", pre / "1ST_READ.BIN",
       *([] if not WIN else ["--coverage-exe", APP / "bin" / "test_coverage.exe", "--routes-exe", APP / "bin" / "vs_route.exe"]))
    return work


def make_image(work, out_dir):
    iso = work / "srdc.iso"
    py("tools/dreamcast/disc/isowriter.py", "--boot", APP / "prebuilt" / "IP.BIN", "--lba", "11702", "--volume", "SRDC",
       work / "disc", iso)
    out_dir.mkdir(parents=True, exist_ok=True)
    cdi = out_dir / "SonicRushDC.cdi"
    if cdi.exists():
        cdi.unlink()
    run([tool("cdi4dc"), iso, cdi], quiet=True)
    if not cdi.is_file() or cdi.stat().st_size < 1 << 20:
        raise RuntimeError("cdi4dc did not write the disc image")
    # the intermediate tree and ISO are ~1 GB: drop them, keep the converted assets for a quick rebuild
    shutil.rmtree(work, ignore_errors=True)
    return cdi


def free_space_ok(path, need_gb):
    try:
        free = shutil.disk_usage(path).free
    except OSError:
        return True
    return free >= need_gb * (1 << 30)


def main():
    ap = argparse.ArgumentParser(description="Make the Sonic Rush DC disc from your ROM")
    ap.add_argument("--rom", required=True)
    ap.add_argument("--out", required=True, help="folder for SonicRushDC.cdi")
    a = ap.parse_args()
    t0 = time.time()
    out_dir = Path(a.out)
    try:
        try:
            (APP / "build").mkdir(exist_ok=True)
            probe = APP / "build" / ".write_test"
            probe.write_text("ok")
            probe.unlink()
        except OSError:
            raise Fail("The disc maker cannot write into its own folder.\nMove the whole SonicRushDC folder to your "
                       "Desktop or Documents (not Program Files) and try again.")
        if not free_space_ok(APP, 4):
            raise Fail("Not enough free disk space: the disc maker needs about 4 GB free on this drive.")
        step(1, "Checking your ROM")
        pct(1)
        sha = check_rom(a.rom)
        step(2, "Unpacking the ROM")
        pct(3)
        extract(a.rom, sha)
        step(3, "Converting the game for the Dreamcast (this is the long part)")
        pct(6)
        convert(sha, 6, 86)
        step(4, "Putting the disc together")
        pct(87)
        work = make_disc(out_dir)
        step(5, "Writing the disc image")
        pct(93)
        cdi = make_image(work, out_dir)
        pct(100)
        say("finished in %d s" % (time.time() - t0))
        say("@@DONE %s" % cdi)
        return 0
    except Fail as e:
        say("@@ERROR %s" % str(e).replace("\n", "\\n"))
        return 2
    except Exception as e:   # noqa: BLE001 - shown to the player with the log
        traceback.print_exc(file=sys.stdout)
        sys.stdout.flush()
        say("@@ERROR Something went wrong while making the disc.\\n\\n%s" %
            str(e).strip().splitlines()[0][:300].replace("\n", " "))
        return 1


if __name__ == "__main__":
    sys.exit(main())
