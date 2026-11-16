#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[0x1E - 4];
    u16 unk1E;
    u16 unk20;
    u8 pad22[0x3A - 0x22];
    s8 unk3A;
    u8 pad3B;
    s32 unk3C;
    s32 unk40;
    u8 pad44[0x4C - 0x44];
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    u8 pad5C[0x64 - 0x5C];
} UnkPlayer_0233a118;

typedef struct {
    UnkPlayer_0233a118 p[2];
    u32 flags;
    u32 padCC;
    s32 unkD0;
    s32 unkD4;
    s32 unkD8;
    u8 padDC[0x114 - 0xDC];
    s32 unk114;
    s32 unk118;
    s32 unk11C;
    s32 unk120;
} UnkStruct_0233a118;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_0233a118_4560;

typedef struct {
    u8 pad0[0x1C];
    s32 unk1C;
    u32 unk20;
    u8 pad24[0x30 - 0x24];
    s32 unk30;
    u32 unk34;
} UnkStruct_0233a118_4590;

extern UnkStruct_0233a118 data_ov000_0236aad8;
extern UnkStruct_0233a118_4560 data_022c4560;
extern UnkStruct_0233a118_4590 data_022c4590;

s32 func_ov000_0233b078(s32 v);

void func_ov000_0233a118(void) {
    s32 v;
    u32 lim;
    u32 i;
    UnkPlayer_0233a118 *p;

    data_ov000_0236aad8.flags |= 1;
    data_ov000_0236aad8.flags &= ~4;
    data_ov000_0236aad8.unkD0 = 0x5000;
    data_ov000_0236aad8.p[0].unk3A = 0;
    data_ov000_0236aad8.p[1].unk3A = -1;
    data_ov000_0236aad8.p[0].unk3C = 0x8000;
    data_ov000_0236aad8.p[0].unk40 = 0x6000;
    if (!(data_ov000_0236aad8.flags & 4)) {
        if (data_022c4560.unk18 & 0x40) {
            v = data_022c4590.unk30;
            lim = data_022c4590.unk34;
        } else {
            v = data_022c4590.unk1C;
            lim = data_022c4590.unk20;
        }
        if ((u32)(func_ov000_0233b078(v << 8) >> 8) >= lim) {
            *(vu16 *)0x4000304 |= 0x8000;
            data_ov000_0236aad8.flags &= ~2;
            data_ov000_0236aad8.unkD4 = 0;
            data_ov000_0236aad8.unkD8 = 0x11000;
        } else {
            *(vu16 *)0x4000304 &= ~0x8000;
            data_ov000_0236aad8.flags |= 2;
            data_ov000_0236aad8.unkD4 = 0;
            data_ov000_0236aad8.unkD8 = -0x11000;
        }
    }
    p = data_ov000_0236aad8.p;
    for (i = 0; i < 2; i++, p++) {
        p->flags |= 0xF;
        p->flags |= 0x30000;
        p->unk1E = 0x100;
        p->unk20 = 0x100;
        p->unk4C = data_ov000_0236aad8.unk114;
        p->unk50 = data_ov000_0236aad8.unk118;
        p->unk54 = data_ov000_0236aad8.unk11C;
        p->unk58 = data_ov000_0236aad8.unk120;
    }
}
