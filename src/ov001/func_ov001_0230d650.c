#include <nitro/types.h>

s32 func_0200225c(s32 a, s32 b);

u16 func_ov001_0230d650(s32 a, s32 b) {
    s32 t = (s32)(((s64)a * 0x3244 + 0x800) >> 12);
    s32 d = func_0200225c(0x8000000, t);
    return (u16)((s32)(((s64)d * b + 0x800) >> 12) >> 12);
}
