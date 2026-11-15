#include <nitro/types.h>

typedef struct { u8 unk0[2]; u8 texIdx; u8 plttIdx; } UnkInfo_020281d0;
typedef struct { fx32 frame; u32 unk4; void *resAnm; u8 unkC[8]; void *resTex; } UnkAnmObj_020281d0;

UnkInfo_020281d0 *func_020261d4(void *anm, u16 idx, u16 frame);
const void *func_02026264(void *anm, u32 idx);
const void *func_02026254(void *anm, u32 idx);
void func_020282b0(void *tex, const void *name, void *res);
void func_02028258(void *tex, const void *name, void *res);

void func_020281d0(void *res, UnkAnmObj_020281d0 *obj, u32 idx) {
    void *anm = obj->resAnm;
    UnkInfo_020281d0 *info = func_020261d4(anm, (u16)idx, (u16)(obj->frame >> 12));
    func_020282b0(obj->resTex, func_02026264(anm, info->texIdx), res);
    if (info->plttIdx != 0xFF) {
        func_02028258(obj->resTex, func_02026254(anm, info->plttIdx), res);
    }
}
