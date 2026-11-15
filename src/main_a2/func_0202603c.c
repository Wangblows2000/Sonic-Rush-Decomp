#include <nitro/types.h>

typedef struct { u32 val[4]; } UnkResName_0202603c;
typedef struct { u8 refBit; u8 idxLeft; u8 idxRight; u8 idxEntry; } UnkPtNode_0202603c;
typedef struct { u8 revision; u8 numEntry; u16 sizeDictBlk; u16 dummy; u16 ofsEntry; UnkPtNode_0202603c node[1]; } UnkDict_0202603c;
typedef struct { u16 sizeUnit; u16 ofsName; u8 data[4]; } UnkDictEntry_0202603c;

void *func_0202603c(const UnkDict_0202603c *dict, const UnkResName_0202603c *name) {
    if (dict->numEntry < 16) {
        u32 i;
        u32 n0 = name->val[0];
        u32 n1 = name->val[1];
        u32 n2 = name->val[2];
        u32 n3 = name->val[3];
        for (i = 0; i < dict->numEntry; i++) {
            const UnkDictEntry_0202603c *e = (const UnkDictEntry_0202603c *)((u8 *)dict + dict->ofsEntry);
            const UnkResName_0202603c *nm = (const UnkResName_0202603c *)((u8 *)e + e->ofsName) + i;
            if (nm->val[0] == n0 && nm->val[1] == n1 && nm->val[2] == n2 && nm->val[3] == n3) {
                const UnkDictEntry_0202603c *e2 = (const UnkDictEntry_0202603c *)((u8 *)dict + dict->ofsEntry);
                return (void *)(e2->data + e2->sizeUnit * i);
            }
        }
    } else {
        const UnkPtNode_0202603c *base = dict->node;
        if (base->idxLeft != 0) {
            const UnkPtNode_0202603c *prev = base;
            const UnkPtNode_0202603c *p = &base[base->idxLeft];
            while (prev->refBit > p->refBit) {
                prev = p;
                p = &base[(&p->idxLeft)[(name->val[p->refBit >> 5] >> (p->refBit & 0x1F)) & 1]];
            }
            {
                const UnkDictEntry_0202603c *e = (const UnkDictEntry_0202603c *)((u8 *)dict + dict->ofsEntry);
                const UnkResName_0202603c *nm = (const UnkResName_0202603c *)((u8 *)e + e->ofsName) + p->idxEntry;
                if (nm->val[0] == name->val[0] && nm->val[1] == name->val[1] && nm->val[2] == name->val[2] &&
                    nm->val[3] == name->val[3]) {
                    return (void *)(e->data + e->sizeUnit * p->idxEntry);
                }
            }
        }
    }
    return NULL;
}
