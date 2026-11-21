#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2d4];
    u16 bufA[0xc0];
    u16 bufB[0xc0];
    void *reg;
    s32 unk5d8;
    s32 unk5dc;
} UnkStruct_022d37ac;

typedef struct {
    void *reg;
    void *bufA;
    void *bufB;
    s32 unkc;
    s32 unk10;
} UnkDma_022d37ac;

extern UnkDma_022d37ac data_02087cdc;
extern u32 gSystemFlags;

#pragma thumb on
void TryAgain_InitSplitScroll(UnkStruct_022d37ac *p) {
    s32 i;

    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1200;
    p->unk5dc = 8;
    p->reg = (void *)0x4001012;
    p->unk5d8 = 0;
    for (i = 0; i < 0x60; i++) {
        p->bufB[i] = -0x60;
        p->bufA[i] = p->bufB[i];
    }
    for (; i < 0xc0; i++) {
        p->bufB[i] = 0x60;
        p->bufA[i] = p->bufB[i];
    }
    data_02087cdc.reg = p->reg;
    data_02087cdc.bufA = p->bufA;
    data_02087cdc.bufB = p->bufB;
    data_02087cdc.unkc = 2;
    data_02087cdc.unk10 = 0;
    gSystemFlags |= 0x400;
}
#pragma thumb reset
