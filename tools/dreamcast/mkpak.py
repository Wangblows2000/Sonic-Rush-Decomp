#!/usr/bin/env python3
"""Pack the small files the game loads per stage into one file (dreamcast/game/engine/pak.c).

A stage load used to open ~180 files (sprite frames.bin / palettes.bin / atlas pages, object
hit boxes); on the GD-ROM every open is an ISO 9660 directory walk (about 30 ms each in
Flycast, 5.3 s for z41's objects). The game opens this pack once and reads each file with a
seek and one read.

  mkpak.py DISC_DIR OUT.pak SUBDIR [SUBDIR ...] [--drop-packed] [--skip-argb-with-pt4]

SUBDIR: directories under DISC_DIR to pack (e.g. sprites objects/hbx ui/res special/sprites
boss/extra/sprites). Keys are paths relative to DISC_DIR ("sprites/act__x/frames.bin").
--skip-argb-with-pt4: leave out atlas<N>.argb1555 when atlas<N>.pt4 exists (the game draws the
PAL4 page; the ARGB1555 file is the host tests' copy). --drop-packed: delete the packed files
(and those skipped) from DISC_DIR: the pack is authoritative for its directories.

"SPAK" v1, little-endian:
  header 16 B   char magic[4] = "SPAK"; u32 version = 1; u32 n_files; u32 n_dirs
  dirs          n_dirs x char[64] (NUL-padded): the packed directories ("sprites/")
  index         n_files x {u32 hash; u32 offset; u32 size}, sorted by hash
                (hash = FNV-1a 32 of the key; the tool refuses collisions)
  data          files, each at a 32-byte aligned offset, in load order: per directory
                frames.bin, (objects/hbx/<id>.hbx), palettes.bin, the rest by name
"""
import os
import struct
import sys


def fnv1a(s):
    h = 0x811C9DC5
    for b in s.encode():
        h = ((h ^ b) * 0x01000193) & 0xFFFFFFFF
    return h


def main(argv):
    opts = [a for a in argv if a.startswith("--")]
    args = [a for a in argv if not a.startswith("--")]
    if len(args) < 3:
        print(__doc__)
        return 2
    disc, out, subdirs = args[0], args[1], args[2:]
    files, skipped = [], []
    # data layout = load order, so the read-ahead (engine/pak.c) serves a whole resource from
    # one read: frames.bin, the object hit boxes of the same id, palettes.bin, the pages
    first = {"frames.bin": 0, "palettes.bin": 2}
    for sd in subdirs:
        root = os.path.join(disc, sd)
        for dp, dn, fn in sorted(os.walk(root)):
            dn.sort()
            for f in sorted(fn, key=lambda n: (first.get(n, 3), n)):
                p = os.path.join(dp, f)
                rel = os.path.relpath(p, disc).replace(os.sep, "/")
                if "--skip-argb-with-pt4" in opts and f.startswith("atlas") and f.endswith(".argb1555") and \
                        os.path.exists(os.path.join(dp, f[:-len(".argb1555")] + ".pt4")):
                    skipped.append(p)
                    continue
                files.append((rel, p))
    # objects/hbx/<id>.hbx right after sprites/<id>/frames.bin
    hbx = {rel: (rel, p) for rel, p in files if rel.startswith("objects/hbx/")}
    ordered, keys = [], {rel for rel, _ in files}
    for rel, p in files:
        if rel.startswith("objects/hbx/") and ("sprites/%s/frames.bin" % rel[12:-4]) in keys:
            continue   # placed with its sprite directory
        ordered.append((rel, p))
        if rel.startswith("sprites/") and rel.endswith("/frames.bin"):
            h = "objects/hbx/%s.hbx" % rel[8:-11]
            if h in hbx:
                ordered.append(hbx[h])
    files = ordered
    hashes = {}
    for rel, _ in files:
        h = fnv1a(rel)
        if h in hashes:
            raise SystemExit("hash collision: %s / %s" % (rel, hashes[h]))
        hashes[h] = rel
    dirs = [d.strip("/") + "/" for d in subdirs]
    hdr = struct.pack("<4sIII", b"SPAK", 1, len(files), len(dirs))
    hdr += b"".join(d.encode().ljust(64, b"\0")[:64] for d in dirs)
    off = len(hdr) + 12 * len(files)
    off = (off + 31) & ~31
    index, blobs = [], []
    for rel, p in files:
        data = open(p, "rb").read()
        index.append((fnv1a(rel), off, len(data)))
        blobs.append((off, data))
        off = (off + len(data) + 31) & ~31
    index.sort()
    with open(out, "wb") as f:
        f.write(hdr + b"".join(struct.pack("<III", *e) for e in index))
        for o, data in blobs:
            f.write(b"\0" * (o - f.tell()))
            f.write(data)
    if "--drop-packed" in opts:
        for _, p in files:
            os.remove(p)
        for p in skipped:
            os.remove(p)
        for sd in subdirs:   # directories left empty
            for dp, dn, fn in sorted(os.walk(os.path.join(disc, sd)), reverse=True):
                if not os.listdir(dp):
                    os.rmdir(dp)
    print("%s: %d files from %s, %d bytes (%d ARGB1555 copies left out)"
          % (out, len(files), " ".join(dirs), off, len(skipped)))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
