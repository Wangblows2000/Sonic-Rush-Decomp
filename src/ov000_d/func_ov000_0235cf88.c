#include <nitro/types.h>

typedef struct {
    u16 x;
    u16 y;
} UnkOfs_0235cf88;

extern u32 data_02087cb8;
extern UnkOfs_0235cf88 data_02087d04[4];

#pragma thumb on

void func_ov000_0235cf88(void) {
    data_02087cb8 &= ~0x400;
    data_02087d04[2].x = 0;
    data_02087d04[2].y = 0;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1c00;
}

#pragma thumb reset
