#!/usr/bin/env python3
"""Vs CPU racing lines: inventory (durable manifest) of build/dreamcast/vs/routes.

For every Battle Play stage / character pair: the line file's header (format version, generation key,
tool build, complete or partial, lines, points), the key the present simulation would give it
(`vs_route ... -key`), and with --replay whether its line 0 still replays to the goal frame for frame in
the present simulation. Writes routes/manifest.json and routes/manifest.md (atomically) and prints the
table. Linux and Windows (pass --exe vs_route.exe).

usage: vs_manifest.py [--build build/dreamcast] [--exe PATH] [--replay]
"""
import argparse
import json
import os
import struct
import subprocess
import sys
import time
from pathlib import Path

STAGES = ["z11", "z12", "z21", "z22", "z31", "z32", "z41", "z42", "z51", "z52", "z61", "z62", "z71", "z72"]
NAMES = {"z1": "Leaf Storm", "z2": "Water Palace", "z3": "Mirage Road", "z4": "Night Carnival",
         "z5": "Huge Crisis", "z6": "Altitude Limit", "z7": "Dead Line"}


def header(path):
    b = path.read_bytes()
    if len(b) < 16 or b[:4] != b"VSRT":
        return None
    ver = struct.unpack_from("<H", b, 4)[0]
    if ver >= 4:
        _, ver, z, a, c, flags, nl, pts, key, tool = struct.unpack_from("<4sHBBBBHIII", b, 0)
        lsz = 16 if ver == 4 else 20
        first, count, rest = struct.unpack_from("<III", b, 24)
        return dict(version=ver, flags=flags, lines=nl, points=pts, key="%08x" % key,
                    tool="%08x" % tool if ver >= 5 else "", line0_frames=count, complete=not (flags & 1),
                    bytes=len(b))
    return dict(version=ver, complete=None, bytes=len(b))


def run(exe, b, s, c, f, *opt):
    r = subprocess.run([exe, "stages", "player", "objects", os.path.join("sprites", "bac"), s, str(c), f] + list(opt),
                       cwd=b, capture_output=True, text=True)
    return r.returncode, r.stdout.strip().splitlines()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--build", default="build/dreamcast")
    ap.add_argument("--exe", default=None)
    ap.add_argument("--replay", action="store_true")
    o = ap.parse_args()
    b = Path(o.build).resolve()
    exe = str(Path(o.exe).resolve()) if o.exe else str(b / "vs" / "vs_route")
    rdir = b / "vs" / "routes"
    out = []
    for s in STAGES:
        for c in (0, 1):
            pair = "%s_%d" % (s, c)
            f = rdir / (pair + ".rte")
            e = dict(pair=pair, stage=NAMES[s[:2]] + " act " + s[2], chara="Blaze" if c else "Sonic")
            rc, lines = run(exe, b, s, c, "-", "-key")
            e["present_key"] = lines[-1].split()[-1] if rc == 0 and lines else ""
            if not f.is_file():
                e["status"] = "missing"
            else:
                h = header(f) or {}
                e.update(h)
                rep = rdir / (pair + ".rte.txt")
                if rep.is_file():
                    e["report"] = rep.read_text(errors="replace").splitlines()[0]
                if h.get("complete") is None:
                    e["status"] = "unreadable"
                else:
                    e["status"] = "complete" if h["complete"] else "partial"
                    e["key_current"] = h.get("key") == e["present_key"]
                if o.replay and h.get("version", 0) >= 4:
                    rc, lines = run(exe, b, s, c, os.path.relpath(f, b), "-replay")
                    e["replay"] = lines[-1] if lines else ""
                    e["replays_to_goal"] = rc == 0
            out.append(e)
    data = dict(made=time.strftime("%Y-%m-%d %H:%M:%S"), routes=str(rdir), pairs=out)
    tmp = rdir / "manifest.json.tmp"
    tmp.write_text(json.dumps(data, indent=1))
    tmp.replace(rdir / "manifest.json")
    md = ["| pair | stage | chara | status | line 0 frames | lines | key current | replays |", "|---|---|---|---|---|---|---|---|"]
    for e in out:
        md.append("| %s | %s | %s | %s | %s | %s | %s | %s |" % (e["pair"], e["stage"], e["chara"], e["status"], e.get("line0_frames", ""),
                  e.get("lines", ""), {True: "yes", False: "no"}.get(e.get("key_current"), "-"),
                  {True: "yes", False: "NO"}.get(e.get("replays_to_goal"), "-")))
    tmp = rdir / "manifest.md.tmp"
    tmp.write_text("\n".join(md) + "\n")
    tmp.replace(rdir / "manifest.md")
    print("\n".join(md))
    n = {k: sum(1 for e in out if e["status"] == k) for k in ("complete", "partial", "missing", "unreadable")}
    print("complete %(complete)d, partial %(partial)d, missing %(missing)d, unreadable %(unreadable)d" % n)


if __name__ == "__main__":
    sys.exit(main())
