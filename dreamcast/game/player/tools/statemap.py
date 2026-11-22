#!/usr/bin/env python3
# Extract Player_State_* / Player_Action_* transitions from the decomp.
import re, os, sys
D=sys.argv[1]
SLOT={'334':'idle','338':'move','33c':'crouch','33C':'crouch','340':'jump','344':'airSpecial','348':'slot348'}
rows=[]
for f in sorted(os.listdir(D)):
    if not re.match(r'Player_(State|Action)_\w+\.c$',f): continue
    name=f[:-2]; s=open(os.path.join(D,f)).read()
    body=s[s.find(name+'('):] if (name+'(') in s else s
    # strip prototypes
    body='\n'.join(l for l in body.split('\n') if not re.match(r'^(extern|void|s32|BOOL|u16|s16)\s.*\);$',l))
    st=sorted(set(re.findall(r'=\s*(?:\(u32\)\s*)?(Player_State_\w+)',body)))
    calls=sorted(set(x for x in re.findall(r'(Player_Action_\w+)\s*\(',body) if x!=name))
    ind=set()
    for m in re.finditer(r'unk(33[4cC8]|34[048])\s*\(|\+ 0x(33[4c8]|34[048])\)\)\(|\(w \+ 0x(33[4c]|34[048])\)\)\(',body):
        k=[g for g in m.groups() if g][0]; ind.add(SLOT.get(k,k))
    for m in re.finditer(r'->(state|unk68)\s*=\s*(\w+)',body): st.append(m.group(2))
    anims=sorted(set(re.findall(r'Player_SetAnimation\([^,]+,\s*(0x[0-9a-fA-F]+|\d+)',body)),key=lambda v:int(v,0))
    rows.append((name,sorted(set(st)),calls,sorted(ind),anims))
for r in rows:
    print('|'.join([r[0],' '.join(x.replace('Player_','') for x in r[1]),' '.join(x.replace('Player_','') for x in r[2]),' '.join(r[3]),' '.join(r[4])]))
