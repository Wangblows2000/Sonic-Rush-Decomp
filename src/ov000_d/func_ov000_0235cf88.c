#include <nitro/types.h>

typedef struct {
    u16 x;
    u16 y;
} UnkOfs_0235cf88;

extern u32 gSystemFlags;
extern UnkOfs_0235cf88 gDisplaySub[4];

#pragma thumb on

void func_ov000_0235cf88(void) {
    gSystemFlags &= ~0x400;
    gDisplaySub[2].x = 0;
    gDisplaySub[2].y = 0;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1c00;
}

#pragma thumb reset
