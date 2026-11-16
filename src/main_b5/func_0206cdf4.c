#include <nitro/types.h>

typedef struct {
    u16 *dst;
    u16 *src;
    void *res;
} UnkPal_0206cdf4;

u32 func_02021ad8(void *res);
u16 func_0206d100(u16 c, s32 a, s32 b, s32 d);
u32 func_02038018(void *res, s32 a);
void func_02035704(void *p, u16 size, s32 b, u32 c);

void func_0206cdf4(UnkPal_0206cdf4 *p, s32 a, s32 b, s32 d) {
    u16 *dst;
    u16 *src;
    u32 i = 0;
    u32 size;

    src = p->src;
    dst = p->dst;
    for (; i < func_02021ad8(p->res) >> 1; i++)
        dst[i] = func_0206d100(src[i], a, b, d);
    size = func_02021ad8(p->res);
    func_02035704(dst, size >> 1, 5, func_02038018(p->res, 0));
}
