#include <nitro/types.h>

typedef struct {
    u16 x;
    u16 y;
} UnkOfs_0235cec8;

extern u32 gSystemFlags;
extern UnkOfs_0235cec8 gDisplaySub[4];

#pragma thumb on

void ResultsScreen_DisableLineScrollHidden(void) {
    gSystemFlags &= ~0x400;
    gDisplaySub[2].x = 0;
    gDisplaySub[2].y = 0xc0;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1800;
}

#pragma thumb reset
