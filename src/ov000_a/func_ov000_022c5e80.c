#include <nitro/types.h>

extern s16 data_0206e470[];

s32 func_0200225c(s32 a, s32 b);

void func_ov000_022c5e80(s32 len, s32 b, s32 a, s32 *outX, s32 *outY, u16 *outAng) {
    u16 ang = func_0200225c(a, b) << 4;
    s32 s = data_0206e470[((ang & 0xffff) >> 4) * 2];
    s32 c = data_0206e470[((ang & 0xffff) >> 4) * 2 + 1];
    s32 x = (s32)(((s64)len * s + 0x800) >> 12);
    s32 y = (s32)(((s64)len * c + 0x800) >> 12);
    if (outX != NULL)
        *outX = x;
    if (outY != NULL)
        *outY = y;
    if (outAng != NULL)
        *outAng = ang;
}
