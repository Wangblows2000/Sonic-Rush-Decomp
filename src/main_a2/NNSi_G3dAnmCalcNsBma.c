#include <nitro/types.h>

typedef struct { u8 revision; u8 numEntry; u16 sizeDictBlk; u16 dummy; u16 ofsEntry; } UnkDict_0202798c;
typedef struct { u16 sizeUnit; u16 ofsName; u8 data[4]; } UnkDictEntry_0202798c;
typedef struct { u8 unk0[8]; UnkDict_0202798c dict; } UnkResAnm_0202798c;
typedef struct { fx32 frame; u32 unk4; UnkResAnm_0202798c *resAnm; } UnkAnmObj_0202798c;
typedef struct { u32 flag; u32 prmMatColor0; u32 prmMatColor1; u32 prmPolygonAttr; } UnkMatResult_0202798c;

u32 func_02027c0c(UnkResAnm_0202798c *anm, u32 info, s32 frame);
u32 func_02027b18(UnkResAnm_0202798c *anm, u32 info, s32 frame);

static inline void *GetData_0202798c(UnkDict_0202798c *dict, u32 idx) {
    UnkDictEntry_0202798c *e = (UnkDictEntry_0202798c *)((u8 *)dict + dict->ofsEntry);
    return e->data + e->sizeUnit * idx;
}

void NNSi_G3dAnmCalcNsBma(UnkMatResult_0202798c *res, UnkAnmObj_0202798c *obj, u32 idx) {
    UnkResAnm_0202798c *anm = obj->resAnm;
    s32 frame;
    u32 *data;
    frame = obj->frame >> 12;
    data = GetData_0202798c(&anm->dict, (u16)idx);
    u32 a;
    u32 b;
    a = func_02027c0c(anm, data[0], frame);
    b = func_02027c0c(anm, data[1], frame);
    res->prmMatColor0 = (res->prmMatColor0 & 0x8000) | a | (b << 16);
    a = func_02027c0c(anm, data[3], frame);
    b = func_02027c0c(anm, data[2], frame);
    res->prmMatColor1 = (res->prmMatColor1 & 0x8000) | b | (a << 16);
    a = func_02027b18(anm, data[4], frame);
    res->prmPolygonAttr = (res->prmPolygonAttr & ~0x1F0000) | (a << 16);
}
