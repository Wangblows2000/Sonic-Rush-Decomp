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
} UnkPlayer_0233a010;

typedef struct {
    UnkPlayer_0233a010 p[2];
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
} UnkStruct_0233a010;

extern UnkStruct_0233a010 data_ov000_0236aad8;
extern UnkPlayer_0233a010 data_ov000_0236ab3c;

#define PLAYER2 ((UnkPlayer_0233a010 *)(u32)&data_ov000_0236ab3c)

void func_ov000_0233a010(void) {
    u32 i;
    UnkPlayer_0233a010 *p;

    data_ov000_0236aad8.flags &= ~1;
    data_ov000_0236aad8.flags |= 4;
    data_ov000_0236aad8.p[0].unk3A = 0;
    data_ov000_0236aad8.p[0].unk3C = 0x8000;
    data_ov000_0236aad8.p[0].unk40 = 0x6000;
    data_ov000_0236aad8.p[1].unk3A = 0;
    data_ov000_0236aad8.p[1].unk3C = 0x8000;
    data_ov000_0236aad8.p[1].unk40 = 0x5800;
    *(vu16 *)0x4000304 |= 0x8000;
    data_ov000_0236aad8.unkD4 = 0;
    data_ov000_0236aad8.flags &= ~2;
    data_ov000_0236aad8.unkD8 = 0;
    p = data_ov000_0236aad8.p;
    for (i = 0; i < 2; i++, p++) {
        p->flags |= 0xF;
        p->flags |= 0x30000;
        p->unk4C = data_ov000_0236aad8.unk114;
        p->unk50 = data_ov000_0236aad8.unk118;
        p->unk54 = data_ov000_0236aad8.unk11C;
        p->unk58 = data_ov000_0236aad8.unk120;
    }
    data_ov000_0236aad8.p[0].unk3A = 0;
    data_ov000_0236aad8.p[0].unk3C = 0x8000;
    data_ov000_0236aad8.p[0].unk40 = 0x6000;
    data_ov000_0236aad8.p[0].unk1E = 0x100;
    data_ov000_0236aad8.p[0].unk20 = 0x100;
    PLAYER2->unk3A = 0;
    PLAYER2->unk3C = 0x8000;
    PLAYER2->unk40 = 0x5800;
    PLAYER2->unk1E = 0x100;
    PLAYER2->unk20 = 0x100;
}
