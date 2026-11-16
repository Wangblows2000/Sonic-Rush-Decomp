#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    s32 unk4;
    u16 unk8;
    u16 unkA;
} UnkStruct_02369ac0;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0xD5E - 0x210];
    u16 unkD5E;
    u8 padD60[0xD6A - 0xD60];
    u16 unkD6A;
    u8 padD6C[0xDC2 - 0xD6C];
    u16 unkDC2;
    u8 padDC4[0xDEE - 0xDC4];
    u16 unkDEE;
    u8 padDF0[0xE9E - 0xDF0];
    u16 unkE9E;
    u8 padEA0[0x10C4 - 0xEA0];
    s32 unk10C4;
} UnkStruct_022e4568;

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
} UnkStruct_Plr18;

extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern UnkStruct_Plr18 *gPlayers;

void func_ov000_022e4568(UnkStruct_022e4568 *w) {
    s32 x;

    if (data_ov000_02369ac0.unk4 == 0x22) {
        w->unkD6A &= ~4;
        w->unkD6A |= 0x1000;
        w->unkDC2 &= ~4;
        w->unkDC2 |= 0x1000;
        w->unkDEE &= ~4;
        w->unkDEE |= 0x1000;
        return;
    }
    if (w->unkD5E != 0)
        x = 0x14000;
    else
        x = -0x14000;
    if (w->unk20C & 0x4000) {
        w->unkD6A |= 4;
        w->unkD6A &= ~0x1000;
        w->unkE9E |= 4;
        w->unkE9E &= ~0x1000;
        w->unk20C &= ~0x4000;
    } else {
        w->unkD6A &= ~4;
        w->unkE9E &= ~4;
    }
    if (w->unk20C & 0x1000) {
        x = (gPlayers->unk18 << 4) - (w->unk10C4 + x);
        if (x < 0)
            x = -x;
        if (x <= 0x64000) {
            w->unkDC2 |= 4;
            w->unkDC2 &= ~0x1000;
            w->unk20C &= ~0x1000;
            goto next;
        }
    }
    w->unkDC2 &= ~4;
next:
    if (w->unk20C & 0x2000) {
        w->unkDEE |= 4;
        w->unkDEE &= ~0x1000;
        w->unk20C &= ~0x2000;
    } else {
        w->unkDEE &= ~4;
    }
}
