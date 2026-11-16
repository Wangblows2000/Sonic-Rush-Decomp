#include <nitro/types.h>

typedef struct {
    void *reg;
    void *bufA;
    void *bufB;
    s32 unkc;
    s32 idx;
} UnkDma_022d36c0;

typedef struct {
    u16 x;
    u16 y;
} UnkScroll_022d36c0;

typedef struct {
    u8 pad0[0x5d4];
    void *reg;
    s32 unk5d8;
    s32 unk5dc;
} UnkStruct_022d36c0;

extern UnkDma_022d36c0 data_02087cdc;
extern u16 *data_02087ce0[];
extern u32 gSystemFlags;
extern UnkScroll_022d36c0 gDisplaySub;

s32 TryAgain_Interpolate(s32 from, s32 to, s32 len, s32 cur, s32 e);

#pragma thumb on
void func_ov001_022d36c0(UnkStruct_022d36c0 *p) {
    u16 *buf;
    s32 v;
    s32 r;
    u16 a;
    u16 b;
    s32 i;

    buf = data_02087ce0[data_02087cdc.idx];
    v = p->unk5dc * p->unk5d8;
    if (v >= 0x30) {
        *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1300;
    } else {
        *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1200;
    }
    if (v >= 0x60) {
        gSystemFlags &= ~0x400;
        gDisplaySub.x = 0;
        gDisplaySub.y = 0;
        p->reg = NULL;
        return;
    }
    r = TryAgain_Interpolate(0, 0x60, 0x60, v, 0);
    a = r - 0x60;
    b = 0x60 - r;
    for (i = 0; i < 0x60; i++) {
        buf[i] = a;
    }
    for (; i < 0xc0; i++) {
        buf[i] = b;
    }
    gSystemFlags |= 0x1000;
    p->unk5d8++;
}
#pragma thumb reset
