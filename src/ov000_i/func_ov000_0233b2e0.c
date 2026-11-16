#include <nitro/types.h>

void func_ov000_0233b330(void *a, s32 *x, s32 *y);

void func_ov000_0233b2e0(void *a, s32 x, s32 y, s16 *ox, s16 *oy) {
    s32 bx, by;

    func_ov000_0233b330(a, &bx, &by);
    *ox = (x - bx) >> 8;
    *oy = (y - by) >> 8;
}
