import csv,sys,bisect,re
import pathlib
R = str(pathlib.Path(__file__).resolve().parents[2])
fs=[]
for l in open(R+'/config/eur/arm9/symbols.txt'):
    m=re.match(r'(\S+) kind:function\(\w+,size=0x([0-9a-f]+)\) addr:0x([0-9a-f]+)',l)
    if m: a=int(m[3],16); fs.append((a,a+int(m[2],16),m[1]))
fs.sort(); st=[f[0] for f in fs]
tgt=[int(x,16) for x in sys.argv[2].split(',')]; span=int(sys.argv[3],16) if len(sys.argv)>3 else 1
hits=set()
for r in csv.DictReader(open(f'{R}/build/eur/ghidra/{sys.argv[1]}/xrefs.csv')):
    ta=int(r['to'],16); fa=int(r['from'],16)
    if any(t<=ta<t+span for t in tgt):
        i=bisect.bisect_right(st,fa)-1
        if i>=0 and fs[i][0]<=fa<fs[i][1]: hits.add(fs[i][0])
        else: hits.add(fa)
h=sorted(hits); print(len(h), ' '.join(hex(x) for x in h))
