#include <nitro/types.h>

typedef struct { u32 val[4]; } UnkResName_02025edc;
typedef struct { u8 refBit; u8 idxLeft; u8 idxRight; u8 idxEntry; } UnkPtNode_02025edc;
typedef struct { u8 revision; u8 numEntry; u16 sizeDictBlk; u16 dummy; u16 ofsEntry; UnkPtNode_02025edc node[1]; } UnkDict_02025edc;
typedef struct { u16 sizeUnit; u16 ofsName; } UnkDictEntry_02025edc;

s32 NNS_G3dGetResDictIdxByName(const UnkDict_02025edc *dict, const UnkResName_02025edc *name) {
    if (dict->numEntry < 16) {
        u32 i;
        u32 n0 = name->val[0];
        u32 n1 = name->val[1];
        u32 n2 = name->val[2];
        u32 n3 = name->val[3];
        for (i = 0; i < dict->numEntry; i++) {
            const UnkDictEntry_02025edc *e = (const UnkDictEntry_02025edc *)((u8 *)dict + dict->ofsEntry);
            const UnkResName_02025edc *nm = (const UnkResName_02025edc *)((u8 *)e + e->ofsName) + i;
            if (nm->val[0] == n0 && nm->val[1] == n1 && nm->val[2] == n2 && nm->val[3] == n3) return i;
        }
    } else {
        const UnkPtNode_02025edc *base = dict->node;
        if (base->idxLeft != 0) {
            const UnkPtNode_02025edc *prev = base;
            const UnkPtNode_02025edc *p = &base[base->idxLeft];
            while (prev->refBit > p->refBit) {
                prev = p;
                p = &base[(&p->idxLeft)[(name->val[p->refBit >> 5] >> (p->refBit & 0x1F)) & 1]];
            }
            {
                const UnkDictEntry_02025edc *e = (const UnkDictEntry_02025edc *)((u8 *)dict + dict->ofsEntry);
                const UnkResName_02025edc *nm = (const UnkResName_02025edc *)((u8 *)e + e->ofsName) + p->idxEntry;
                if (nm->val[0] == name->val[0] && nm->val[1] == name->val[1] && nm->val[2] == name->val[2] &&
                    nm->val[3] == name->val[3]) {
                    return p->idxEntry;
                }
            }
        }
    }
    return -1;
}
