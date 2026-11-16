#include <nitro/types.h>

extern s16 gDisplayMain[];

#pragma thumb on
void TitleDemo_ShowBg2At(void *p, s32 x, s32 y) {
    u32 v;

    gDisplayMain[4] = -x;
    gDisplayMain[4 + 1] = -y;
    v = (*(vu32 *)0x4000000 & 0x1f00) >> 8;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | ((v | 4) << 8);
}
#pragma thumb reset
