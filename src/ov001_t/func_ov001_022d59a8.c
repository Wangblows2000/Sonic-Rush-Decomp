#include <nitro/types.h>

extern s16 data_0208922c[];

#pragma thumb on
void func_ov001_022d59a8(void *p, s32 x, s32 y) {
    u32 v;

    data_0208922c[6] = -x;
    data_0208922c[6 + 1] = -y;
    v = (*(vu32 *)0x4000000 & 0x1f00) >> 8;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | ((v | 8) << 8);
}
#pragma thumb reset
