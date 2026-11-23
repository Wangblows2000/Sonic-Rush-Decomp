#!/usr/bin/env python3
"""Minimal ISO 9660 + Joliet writer for Dreamcast data discs (no genisoimage needed).

Equivalent of  genisoimage -C 0,<lba> -V <vol> -G IP.BIN -J -l -o OUT.iso DIR  for what KOS reads:
  - sectors 0..15: the bootstrap (IP.BIN, 32 KB);
  - every extent address is offset by the session start <lba> (11702 for a CDI made by cdi4dc);
  - a primary volume descriptor with 8.3 names and a Joliet supplementary descriptor with the full names
    (KOS fs_iso9660 uses Joliet when present).

  isowriter.py --boot IP.BIN --lba 11702 --volume SRDC DISC_DIR OUT.iso
"""
import argparse
import os
import re
import struct
import time
from pathlib import Path

SECTOR = 2048


def both16(v):
    return struct.pack("<H", v) + struct.pack(">H", v)


def both32(v):
    return struct.pack("<I", v) + struct.pack(">I", v)


def rec_date(t):
    tm = time.gmtime(t)
    return bytes([tm.tm_year - 1900, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, 0])


def vol_date(t):
    tm = time.gmtime(t)
    return time.strftime("%Y%m%d%H%M%S", tm).encode() + b"00" + b"\x00"


def pad_str(s, n, fill=b" "):
    b = s if isinstance(s, bytes) else s.encode("ascii")
    return (b + fill * n)[:n]


class Node:
    def __init__(self, path, name, parent):
        self.path = path            # filesystem path
        self.name = name            # real name (Joliet)
        self.parent = parent
        self.is_dir = path.is_dir()
        self.children = []
        self.size = 0 if self.is_dir else path.stat().st_size
        self.iso_name = b""         # 8.3 identifier (bytes, files with ;1)
        self.jol_name = b""         # UCS-2 BE identifier
        self.lba = 0                # files: data; dirs: primary directory extent
        self.jlba = 0               # dirs: Joliet directory extent
        self.dsize = 0              # dirs: primary directory size (bytes)
        self.jdsize = 0
        self.num = 0                # path table number


def iso_names(children):
    """unique 8.3 d-character identifiers within one directory"""
    used = set()
    for n in children:
        base, ext = (n.name, "") if n.is_dir else os.path.splitext(n.name)
        base = re.sub(r"[^A-Z0-9_]", "_", base.upper())[:8] or "_"
        ext = re.sub(r"[^A-Z0-9_]", "_", ext[1:].upper())[:3]
        k = 0
        while True:
            b = base if k == 0 else (base[:8 - len(str(k)) - 1] + "~" + str(k))
            ident = b if n.is_dir else (b + "." + ext)
            if ident not in used:
                break
            k += 1
        used.add(ident)
        n.iso_name = ident.encode("ascii") if n.is_dir else (ident + ";1").encode("ascii")
        n.jol_name = n.name.encode("utf-16-be")   # no ;1 version in Joliet names (as genisoimage)


def dir_record(ident, lba, size, flags, t):
    l = 33 + len(ident)
    if l % 2:
        l += 1
    r = (bytes([l, 0]) + both32(lba) + both32(size) + rec_date(t) + bytes([flags, 0, 0]) + both16(1) +
         bytes([len(ident)]) + ident)
    return r + b"\x00" * (l - len(r))


def dir_records(node, joliet, base, t):
    """records of one directory extent, padded so no record crosses a sector boundary"""
    parent = node.parent or node
    recs = [dir_record(b"\x00", base + (node.jlba if joliet else node.lba), node.jdsize if joliet else node.dsize, 2, t),
            dir_record(b"\x01", base + (parent.jlba if joliet else parent.lba),
                       parent.jdsize if joliet else parent.dsize, 2, t)]
    kids = sorted(node.children, key=lambda c: c.jol_name if joliet else c.iso_name)
    for c in kids:
        ident = c.jol_name if joliet else c.iso_name
        if c.is_dir:
            recs.append(dir_record(ident, base + (c.jlba if joliet else c.lba), c.jdsize if joliet else c.dsize, 2, t))
        else:
            recs.append(dir_record(ident, base + c.lba, c.size, 0, t))
    out = bytearray()
    for r in recs:
        if len(out) % SECTOR + len(r) > SECTOR:
            out += b"\x00" * (SECTOR - len(out) % SECTOR)
        out += r
    if len(out) % SECTOR:
        out += b"\x00" * (SECTOR - len(out) % SECTOR)
    return bytes(out)


def build_tree(root):
    top = Node(Path(root), "", None)
    stack = [top]
    while stack:
        n = stack.pop()
        for p in sorted(n.path.iterdir(), key=lambda p: p.name):
            c = Node(p, p.name, n)
            n.children.append(c)
            if c.is_dir:
                stack.append(c)
        iso_names(n.children)
    return top


def dirs_bfs(top):
    """directories in path-table order: breadth first, sorted by parent number then identifier"""
    order = [top]
    i = 0
    while i < len(order):
        d = order[i]
        order += sorted([c for c in d.children if c.is_dir], key=lambda c: c.iso_name)
        i += 1
    for k, d in enumerate(order):
        d.num = k + 1
    return order


def path_table(dirs, joliet, big, base):
    out = bytearray()
    for d in dirs:
        ident = b"\x00" if d.parent is None else (d.jol_name if joliet else d.iso_name)
        lba = base + (d.jlba if joliet else d.lba)
        parent = 1 if d.parent is None else d.parent.num
        out += bytes([len(ident), 0])
        out += struct.pack(">I" if big else "<I", lba) + struct.pack(">H" if big else "<H", parent) + ident
        if len(ident) % 2:
            out += b"\x00"
    return bytes(out)


def sectors(n):
    return (n + SECTOR - 1) // SECTOR


def write_iso(disc_dir, out_iso, boot, base=11702, volume="SRDC", t=None):
    t = time.time() if t is None else t
    top = build_tree(disc_dir)
    dirs = dirs_bfs(top)
    # directory sizes do not depend on addresses: size them with dummy addresses first
    for d in dirs:
        d.dsize = len(dir_records(d, False, 0, t))
        d.jdsize = len(dir_records(d, True, 0, t))
    pt_len = len(path_table(dirs, False, False, 0))
    jpt_len = len(path_table(dirs, True, False, 0))
    # layout: 0-15 bootstrap, 16 PVD, 17 Joliet SVD, 18 terminator, path tables, directories, file data
    lba = 19
    pt_l, pt_m = lba, lba + sectors(pt_len)
    lba = pt_m + sectors(pt_len)
    jpt_l, jpt_m = lba, lba + sectors(jpt_len)
    lba = jpt_m + sectors(jpt_len)
    for d in dirs:
        d.lba = lba
        lba += d.dsize // SECTOR
    for d in dirs:
        d.jlba = lba
        lba += d.jdsize // SECTOR
    files = []
    for d in dirs:
        for c in sorted(d.children, key=lambda c: c.iso_name):
            if not c.is_dir:
                c.lba = lba
                lba += max(1, sectors(c.size)) if c.size else 0
                files.append(c)
    total = lba
    root_rec = dir_record(b"\x00", base + top.lba, top.dsize, 2, t)
    jroot_rec = dir_record(b"\x00", base + top.jlba, top.jdsize, 2, t)

    def vd(kind, joliet):
        v = bytearray(SECTOR)
        v[0] = kind
        v[1:6] = b"CD001"
        v[6] = 1
        if joliet:
            v[8:40] = (b"\x00\x20" * 16)[:32]
            v[40:72] = (volume.encode("utf-16-be") + b"\x00\x20" * 16)[:32]
            v[88:91] = b"%/E"
        else:
            v[8:40] = pad_str("", 32)
            v[40:72] = pad_str(volume, 32)
        v[80:88] = both32(base + total)
        v[120:124] = both16(1)
        v[124:128] = both16(1)
        v[128:132] = both16(SECTOR)
        n = jpt_len if joliet else pt_len
        v[132:140] = both32(n)
        v[140:144] = struct.pack("<I", base + (jpt_l if joliet else pt_l))
        v[148:152] = struct.pack(">I", base + (jpt_m if joliet else pt_m))
        v[156:190] = jroot_rec if joliet else root_rec
        blank = (b"\x00\x20" * 64) if joliet else b" " * 128
        for off in (190, 318, 446, 574):
            v[off:off + 128] = blank[:128]
        app = "SONIC RUSH DC DISC MAKER"
        v[574:702] = ((app.encode("utf-16-be") + b"\x00\x20" * 64)[:128]) if joliet else pad_str(app, 128)
        for off in (702, 739, 776):
            v[off:off + 37] = ((b"\x00\x20" * 19)[:37]) if joliet else b" " * 37
        d = vol_date(t)
        v[813:830] = d
        v[830:847] = d
        v[847:864] = b"0" * 16 + b"\x00"
        v[864:881] = d
        v[881] = 1
        return bytes(v)

    with open(out_iso, "wb") as f:
        boot_data = Path(boot).read_bytes()[:16 * SECTOR]
        f.write(boot_data + b"\x00" * (16 * SECTOR - len(boot_data)))
        f.write(vd(1, False))
        f.write(vd(2, True))
        term = bytearray(SECTOR)
        term[0] = 255
        term[1:6] = b"CD001"
        term[6] = 1
        f.write(term)
        for joliet, (l_at, m_at, n) in ((False, (pt_l, pt_m, pt_len)), (True, (jpt_l, jpt_m, jpt_len))):
            for big, at in ((False, l_at), (True, m_at)):
                assert f.tell() == at * SECTOR
                pt = path_table(dirs, joliet, big, base)
                f.write(pt + b"\x00" * (sectors(n) * SECTOR - len(pt)))
        for d in dirs:
            assert f.tell() == d.lba * SECTOR
            f.write(dir_records(d, False, base, t))
        for d in dirs:
            assert f.tell() == d.jlba * SECTOR
            f.write(dir_records(d, True, base, t))
        for c in files:
            if not c.size:
                continue
            assert f.tell() == c.lba * SECTOR
            with open(c.path, "rb") as src:
                while True:
                    chunk = src.read(1 << 20)
                    if not chunk:
                        break
                    f.write(chunk)
            if c.size % SECTOR:
                f.write(b"\x00" * (SECTOR - c.size % SECTOR))
        assert f.tell() == total * SECTOR
    return total


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--boot", required=True, help="IP.BIN (bootstrap, sectors 0..15)")
    ap.add_argument("--lba", type=int, default=11702, help="session start sector (genisoimage -C 0,<lba>)")
    ap.add_argument("--volume", default="SRDC")
    ap.add_argument("disc_dir")
    ap.add_argument("out_iso")
    a = ap.parse_args()
    n = write_iso(a.disc_dir, a.out_iso, a.boot, a.lba, a.volume)
    print("%s: %d sectors" % (a.out_iso, n))


if __name__ == "__main__":
    main()
