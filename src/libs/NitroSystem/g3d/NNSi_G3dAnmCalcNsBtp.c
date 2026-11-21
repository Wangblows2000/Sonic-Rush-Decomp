#include <nitro/types.h>

typedef struct { u8 unk0[2]; u8 texIdx; u8 plttIdx; } UnkInfo_020281d0;
typedef struct { fx32 frame; u32 unk4; void *resAnm; u8 unkC[8]; void *resTex; } UnkAnmObj_020281d0;

UnkInfo_020281d0 *NNSi_G3dGetTexPatAnmFV(void *anm, u16 idx, u16 frame);
const void *NNSi_G3dGetTexPatAnmTexNameByIdx(void *anm, u32 idx);
const void *NNSi_G3dGetTexPatAnmPlttNameByIdx(void *anm, u32 idx);
void SetTexParamaters_(void *tex, const void *name, void *res);
void SetPlttParamaters_(void *tex, const void *name, void *res);

void NNSi_G3dAnmCalcNsBtp(void *res, UnkAnmObj_020281d0 *obj, u32 idx) {
    void *anm = obj->resAnm;
    UnkInfo_020281d0 *info = NNSi_G3dGetTexPatAnmFV(anm, (u16)idx, (u16)(obj->frame >> 12));
    SetTexParamaters_(obj->resTex, NNSi_G3dGetTexPatAnmTexNameByIdx(anm, info->texIdx), res);
    if (info->plttIdx != 0xFF) {
        SetPlttParamaters_(obj->resTex, NNSi_G3dGetTexPatAnmPlttNameByIdx(anm, info->plttIdx), res);
    }
}
