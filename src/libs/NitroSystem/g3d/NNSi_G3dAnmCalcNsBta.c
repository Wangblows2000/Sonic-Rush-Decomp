#include <nitro/types.h>

typedef struct { fx32 frame; u32 unk4; void *resAnm; } UnkAnmObj_02027dbc;
typedef struct { u32 flag; u32 unk4[3]; u32 prmTexImage; } UnkMatResult_02027dbc;

void GetTexSRTAnm_(void *anm, u16 idx, s32 frame, UnkMatResult_02027dbc *res);

void NNSi_G3dAnmCalcNsBta(UnkMatResult_02027dbc *res, UnkAnmObj_02027dbc *obj, u32 idx) {
    GetTexSRTAnm_(obj->resAnm, (u16)idx, obj->frame >> 12, res);
    res->prmTexImage &= ~0xC0000000;
    res->prmTexImage |= 0x40000000;
    res->flag |= 8;
}
