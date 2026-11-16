#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u16 *unk4;
    u16 *unk8;
    s32 unkC;
    s32 unk10;
} UnkStruct_02087cdc;

typedef struct {
    u8 pad0[0x910];
    u16 buf910[0xC0];
    u16 bufA90[0xC0];
    u32 unkC10;
} UnkStruct_0235cf04;

extern UnkStruct_02087cdc data_02087cdc;
extern u32 gSystemFlags;

#pragma thumb on

void func_ov000_0235cf04(UnkStruct_0235cf04 *w) {
    s32 i;

    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1c00;
    w->unkC10 = 0x400101a;
    for (i = 0; i < 0xC0; i++) {
        w->buf910[i] = w->bufA90[i] = 0;
    }
    data_02087cdc.unk0 = w->unkC10;
    data_02087cdc.unk4 = w->buf910;
    data_02087cdc.unk8 = w->bufA90;
    data_02087cdc.unkC = 2;
    data_02087cdc.unk10 = 0;
    gSystemFlags |= 0x400;
}

#pragma thumb reset
