#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_02087cdc;

extern UnkStruct_02087cdc data_02087cdc;
extern u16 *data_02087ce0[];
extern u32 gSystemFlags;

#pragma thumb on

void func_ov000_0235ce3c(void *w, s32 v) {
    u16 *p;
    u16 neg;
    s32 i;
    u16 pos;

    p = data_02087ce0[data_02087cdc.unk10];
    if (v <= 0x3c) {
        *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1c00;
    } else {
        *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1800;
    }
    neg = -v;
    for (i = 0; i < 0x54; i++) {
        p[i] = neg;
    }
    pos = v;
    for (; i < 0xC0; i++) {
        p[i] = pos;
    }
    gSystemFlags |= 0x1000;
}

#pragma thumb reset
