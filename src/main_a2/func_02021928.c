#include <nitro/types.h>

typedef struct { u8 revision; u8 numEntry; u16 sizeDictBlk; u16 dummy; u16 ofsEntry; } UnkDict_02021928;
typedef struct { u16 sizeUnit; u16 ofsName; u8 data[4]; } UnkDictEntry_02021928;
typedef struct { u16 ofsIdx; u8 num; u8 flag; } UnkBind_02021928;
typedef struct { u8 unk0[4]; UnkDict_02021928 dict; } UnkBlk_02021928;
typedef struct { u8 unk0[0x14]; u32 param; u8 unk18[8]; u16 width; u16 height; fx32 magW; fx32 magH; } UnkMat_02021928;
typedef struct { u8 unk0[8]; u32 unk8; u8 unkC[0xC]; u32 unk18; } UnkObj_02021928;

fx32 func_0200225c(fx32 a, fx32 b);

void func_02021928(UnkBlk_02021928 *blk, UnkBind_02021928 *bind, UnkObj_02021928 *obj, u32 *tex) {
    u32 base;
    u8 *idx = (u8 *)blk + bind->ofsIdx;
    u32 i;
    if ((tex[0] & 0x1C000000) != 0x14000000) {
        base = obj->unk8 & 0xFFFF;
    } else {
        base = obj->unk18 & 0xFFFF;
    }
    for (i = 0; i < bind->num; i++) {
        u32 w;
        u32 h;
        UnkDictEntry_02021928 *e = (UnkDictEntry_02021928 *)((u8 *)&blk->dict + blk->dict.ofsEntry);
        UnkMat_02021928 *mat = (UnkMat_02021928 *)((u8 *)blk + *(u32 *)(e->data + e->sizeUnit * idx[i]));
        mat->param |= tex[0] + base;
        w = tex[1] & 0x7FF;
        h = (tex[1] >> 11) & 0x7FF;
        mat->magW = (w == mat->width) ? 0x1000 : func_0200225c(w << 12, mat->width << 12);
        mat->magH = (h == mat->height) ? 0x1000 : func_0200225c(h << 12, mat->height << 12);
    }
    bind->flag |= 1;
}
