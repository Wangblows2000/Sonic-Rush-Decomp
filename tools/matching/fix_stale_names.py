#!/usr/bin/env python3
"""Rewrite default names (func_/data_[ovNNN_]ADDR) in src/ that symbols.txt has
since renamed (e.g. runtime intrinsics) to their current names."""
import glob
import pathlib
import re

cur = {}
for p in glob.glob('config/eur/arm9/**/symbols.txt', recursive=True):
    mod = 'main'
    if '/overlays/' in p:
        mod = p.split('/overlays/')[1].split('/')[0]
    elif '/itcm/' in p:
        mod = 'itcm'
    elif '/dtcm/' in p:
        mod = 'dtcm'
    for line in open(p):
        m = re.match(r'(\S+) kind:\S+ addr:0x([0-9a-f]+)', line)
        if m:
            cur.setdefault((mod, int(m[2], 16)), m[1])
names = set(cur.values())
for f in pathlib.Path('src').rglob('*.c'):
    t = f.read_text()
    o = t
    for kind, ov, addr in set(re.findall(r'\b(func|data)_(ov\d{3}_)?([0-9a-f]{8})\b', t)):
        old = f"{kind}_{ov}{addr}"
        if old in names:
            continue
        mod = ov[:-1] if ov else 'main'
        a = int(addr, 16)
        new = cur.get((mod, a))
        if mod == 'main' and new is None:
            new = cur.get(('itcm', a)) or cur.get(('dtcm', a))
        if new and new != old:
            t = re.sub(rf'\b{old}\b', new, t)
    if t != o:
        f.write_text(t)
        print(f)
