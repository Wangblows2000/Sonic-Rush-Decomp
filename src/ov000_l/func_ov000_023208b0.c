#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u8 padC[0x6c - 0xc];
    s32 unk6C;
} UnkStruct_0236aad8;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_022c4560;

extern UnkStruct_0236aad8 data_ov000_0236aad8;
extern UnkStruct_022c4560 data_022c4560;

void func_ov000_023208b0(UnkStruct_EffObj *w) {
    w->spr0.unk2C &= ~0x80000;
    w->spr1.unk2C &= ~0x80000;
    w->unk10 += w->unk2E;
    if (w->unk10 < -0x4000)
        w->unk10 += 0x20000;
    if (data_ov000_0236aad8.unk8 < data_ov000_0236aad8.unk6C)
        w->spr0.unk2C |= 0x80000;
    else
        w->spr1.unk2C |= 0x80000;
    if (data_022c4560.unk18 & 1)
        w->unk5C &= ~0x20;
}
