#!/usr/bin/env python3
"""Map xrefs/strings to containing functions. Read-only on the repo.
usage: fnmap.py <module> [query...]
  module: main|ov000|ov001
Outputs per-function facts: strings referenced, IO regs, callers, callees."""
import csv, re, sys, collections, bisect
import pathlib
R = str(pathlib.Path(__file__).resolve().parents[2])
mod = sys.argv[1]
symf = {'main': 'config/eur/arm9/symbols.txt'}.get(mod, f'config/eur/arm9/overlays/{mod}/symbols.txt')
funcs = []
names = {}
for path in [symf] + (['config/eur/arm9/symbols.txt'] if mod != 'main' else []) + ['config/eur/arm9/itcm/symbols.txt']:
    for l in open(f'{R}/{path}'):
        m = re.match(r'(\S+) kind:function\(\w+,size=0x([0-9a-f]+)\) addr:0x([0-9a-f]+)', l)
        if m:
            a = int(m[3], 16); funcs.append((a, a + int(m[2], 16), m[1])); names[a] = m[1]
        m = re.match(r'(\S+) kind:(data|bss)', l)
        if m:
            a = int(re.search(r'addr:0x([0-9a-f]+)', l)[1], 16); names.setdefault(a, m[1])
funcs.sort(); starts = [f[0] for f in funcs]
def owner(a):
    i = bisect.bisect_right(starts, a) - 1
    if i >= 0 and funcs[i][0] <= a < funcs[i][1]: return funcs[i]
    return None
strs = {}
for m2 in ['main', mod]:
    with open(f'{R}/build/eur/ghidra/{m2}/strings.csv') as f:
        for r in csv.DictReader(f): strs[int(r['address'], 16)] = r['value']
info = collections.defaultdict(lambda: {'str': set(), 'io': set(), 'callers': set(), 'callees': set(), 'data': set()})
with open(f'{R}/build/eur/ghidra/{mod}/xrefs.csv') as f:
    for r in csv.DictReader(f):
        fa, ta, t = int(r['from'], 16), int(r['to'], 16), r['type']
        o = owner(fa)
        if not o: continue
        n = o[2]
        if ta in strs: info[n]['str'].add(strs[ta])
        elif 0x04000000 <= ta < 0x04800000: info[n]['io'].add(hex(ta))
        elif 'CALL' in t:
            tn = names.get(ta, hex(ta)); info[n]['callees'].add(tn); info[tn]['callers'].add(n)
        elif t in ('DATA', 'READ', 'WRITE', 'PARAM') and ta in names:
            info[n]['data'].add(names[ta])
q = sys.argv[2:]
for a, e, n in funcs:
    d = info[n]
    line = f"{n} size={e-a} callers={len(d['callers'])} callees={len(d['callees'])} str={sorted(d['str'])[:4]} io={sorted(d['io'])[:6]}"
    if not q or any(x in line or x in ' '.join(d['data']) or x in ' '.join(d['callees']) for x in q):
        print(line)
