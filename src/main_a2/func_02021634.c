#include <nitro/types.h>

typedef struct { u8 revision; u8 numEntry; u16 sizeDictBlk; u16 dummy; u16 ofsEntry; } UnkDict_02021634;
typedef struct { u16 sizeUnit; u16 ofsName; u8 data[4]; } UnkDictEntry_02021634;
typedef struct { u8 unk0[8]; u32 ofs; } UnkRes_02021634;
typedef struct { u8 unk0[2]; u16 ofsDict; } UnkBlk_02021634;
typedef struct { u8 unk0[0x34]; u16 ofsDict; } UnkObj_02021634;

void *func_0202603c(void *p, const void *name);
void func_020216e4(void *blk, void *entry, UnkObj_02021634 *obj, void *r);

BOOL func_02021634(UnkRes_02021634 *res, UnkObj_02021634 *obj) {
    UnkBlk_02021634 *blk = (UnkBlk_02021634 *)((u8 *)res + res->ofs);
    UnkDict_02021634 *dict = (UnkDict_02021634 *)((u8 *)blk + blk->ofsDict);
    u32 i;
    BOOL ok = TRUE;
    for (i = 0; i < dict->numEntry; i++) {
        UnkDictEntry_02021634 *e = (UnkDictEntry_02021634 *)((u8 *)dict + dict->ofsEntry);
        void *r = func_0202603c((u8 *)obj + obj->ofsDict, (u8 *)e + e->ofsName + i * 16);
        if (r != NULL) {
            UnkDictEntry_02021634 *e2 = (UnkDictEntry_02021634 *)((u8 *)dict + dict->ofsEntry);
            u8 *data = e2->data + e2->sizeUnit * i;
            if (!(data[3] & 1)) func_020216e4(blk, data, obj, r);
        } else {
            ok = FALSE;
        }
    }
    return ok;
}
