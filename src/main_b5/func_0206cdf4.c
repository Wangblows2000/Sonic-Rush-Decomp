#include <nitro/types.h>

typedef struct {
    u16 *dst;
    u16 *src;
    void *res;
} UnkPal_0206cdf4;

u32 func_02021ad8(void *res);
u16 ObjDraw_ShiftColor(u16 c, s32 a, s32 b, s32 d);
u32 func_02038018(void *res, s32 a);
void PaletteTransfer_QueueCopy(void *p, u16 size, s32 b, u32 c);

void func_0206cdf4(UnkPal_0206cdf4 *p, s32 a, s32 b, s32 d) {
    u16 *dst;
    u16 *src;
    u32 i = 0;
    u32 size;

    src = p->src;
    dst = p->dst;
    for (; i < func_02021ad8(p->res) >> 1; i++)
        dst[i] = ObjDraw_ShiftColor(src[i], a, b, d);
    size = func_02021ad8(p->res);
    PaletteTransfer_QueueCopy(dst, size >> 1, 5, func_02038018(p->res, 0));
}
