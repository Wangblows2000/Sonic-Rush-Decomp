#include <nitro/types.h>

typedef struct { u8 revision; u8 numEntry; u16 sizeDictBlk; u16 dummy; u16 ofsEntry; } UnkDict_020217e4;
typedef struct { u16 sizeUnit; u16 ofsName; u8 data[4]; } UnkDictEntry_020217e4;
typedef struct { u8 unk0[8]; u32 ofs; } UnkRes_020217e4;
typedef struct { u8 unk0[0x3C]; u8 unk3C[4]; } UnkObj_020217e4;

void *NNS_G3dGetResDataByName(void *p, const void *name);
void bindMdlTex_Internal_(void *blk, void *entry, UnkObj_020217e4 *obj, void *r);

BOOL NNS_G3dBindMdlTex(UnkRes_020217e4 *res, UnkObj_020217e4 *obj) {
    u8 *blk = (u8 *)res + res->ofs;
    UnkDict_020217e4 *dict = (UnkDict_020217e4 *)(blk + *(u16 *)blk);
    BOOL ok = TRUE;
    u32 i;
    for (i = 0; i < dict->numEntry; i++) {
        UnkDictEntry_020217e4 *e = (UnkDictEntry_020217e4 *)((u8 *)dict + dict->ofsEntry);
        void *r = NNS_G3dGetResDataByName(obj->unk3C, (u8 *)e + e->ofsName + i * 16);
        if (r != NULL) {
            UnkDictEntry_020217e4 *e2 = (UnkDictEntry_020217e4 *)((u8 *)dict + dict->ofsEntry);
            u8 *data = e2->data + e2->sizeUnit * i;
            if (!(data[3] & 1)) bindMdlTex_Internal_(blk, data, obj, r);
        } else {
            ok = FALSE;
        }
    }
    return ok;
}
