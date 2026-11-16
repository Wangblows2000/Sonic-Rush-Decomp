#include <nitro/types.h>

typedef struct { u8 pad0[0x1c]; u8 unk1C; u8 unk1D; u8 pad1E[2]; u32 unk20; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;

typedef struct { s32 x; s32 y; } UnkVec_0204594c;
typedef struct {
    u8 pad0[4];
    s32 unk4;
    UnkVec_0204594c pos[6]; /* 0x8 */
    s16 unk38;
    s16 unk3A;
    s16 unk3C;
    u8 pad3E[0x16];
    s16 digits[6][6];       /* 0x54 */
    s16 unk9C;
    u8 pad9E[2];
    s32 unkA0;
    s32 unkA4;
    s32 unkA8;
    u32 unkAC;
    u16 unkB0;
    u16 unkB2;
    u16 unkB4;
    u16 unkB6;
    u16 unkB8;
} UnkObj_0204594c;

void MIi_CpuClear16(u16 v, void *dst, u32 size);
s32 FX_DivS32(s32 n, s32 d);
s32 FX_ModS32(s32 n, s32 d);
s16 func_02059a50(u8 a, u8 b, u16 c);

#pragma thumb on
void func_0204594c(UnkObj_0204594c *o) {
    vu16 zero;
    u32 t;
    s32 c;

    o->pos[0].x = -0x72000;
    o->pos[0].y = 0x3c000;
    o->unk38 = 0;
    o->pos[1].x = -0x72000;
    o->pos[1].y = 0x4c000;
    o->unk3A = 0;
    o->pos[2].x = -0x72000;
    o->pos[2].y = 0x69000;
    o->unk3C = 0;
    zero = 0;
    MIi_CpuClear16(zero, o->digits, sizeof(o->digits));
    o->unkAC = data_022c4560.unk20;
    t = o->unkAC;
    if (t < 0x8c9f) {
        o->unkB0 = FX_DivS32(t, 0xe10) + 0x13;
        t = FX_ModS32(t, 0xe10);
        o->unkB2 = FX_DivS32(t, 0x258) + 0x13;
        t = FX_ModS32(t, 0x258);
        o->unkB4 = FX_DivS32(t, 0x3c) + 0x13;
        c = (FX_ModS32(t, 0x3c) * 0x1b1) >> 8;
        if (c >= 100)
            c = 99;
        o->unkB6 = FX_DivS32(c, 10) + 0x13;
        o->unkB8 = FX_ModS32(c, 10) + 0x13;
    } else {
        o->unkB0 = 0x1c;
        o->unkB2 = 0x18;
        o->unkB4 = 0x1c;
        o->unkB6 = 0x1c;
        o->unkB8 = 0x1c;
    }
    o->unk9C = func_02059a50(data_022c4560.unk1C, data_022c4560.unk1D, data_022c4560.unk20);
    if (o->unk9C > 5)
        o->unk9C = 5;
    o->unk9C = o->unk9C * 2;
    o->unkA4 = 0x50000;
    o->unkA8 = 0x7c000;
    o->unkA0 = 0x2000;
    o->unk4 = 0;
}
#pragma thumb reset
