#include <nitro/types.h>

typedef struct {
    u16 *dst;
    u16 *src;
    void *res;
} UnkPal_0206cdf4;

u32 NNS_G3dPlttGetRequiredSize(void *res);
u16 ObjDraw_ShiftColor(u16 c, s32 a, s32 b, s32 d);
u32 Asset3D_GetPaletteKeyByIndex(void *res, s32 a);
void PaletteTransfer_QueueCopy(void *p, u16 size, s32 b, u32 c);

void ModelPalette_ShiftColor(UnkPal_0206cdf4 *p, s32 a, s32 b, s32 d) {
    u16 *dst;
    u16 *src;
    u32 i = 0;
    u32 size;

    src = p->src;
    dst = p->dst;
    for (; i < NNS_G3dPlttGetRequiredSize(p->res) >> 1; i++)
        dst[i] = ObjDraw_ShiftColor(src[i], a, b, d);
    size = NNS_G3dPlttGetRequiredSize(p->res);
    PaletteTransfer_QueueCopy(dst, size >> 1, 5, Asset3D_GetPaletteKeyByIndex(p->res, 0));
}
