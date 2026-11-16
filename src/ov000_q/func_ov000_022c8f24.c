#pragma opt_dead_assignments off
#include <nitro/types.h>

typedef struct UnkStruct_022c8f24_3 {
    u8 pad[0x216];
    u16 unk216;
    u8 pad0[0x26e - 0x218];
    u16 unk26E;
    u8 pad1[0x29a - 0x270];
    u16 unk29A;
    u8 pad2[0x304 - 0x29c];
    s32 unk304;
    u8 pad3[4];
    u8 unk30C[4];
} UnkStruct_022c8f24_3;

typedef struct UnkStruct_022c8f24_2 {
    u8 pad[0x214];
    UnkStruct_022c8f24_3 *unk214;
} UnkStruct_022c8f24_2;

typedef struct UnkStruct_022c8f24_4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c8f24_4;

typedef struct UnkStruct_022c8f24_5 {
    u8 pad[8];
    u16 unk8;
} UnkStruct_022c8f24_5;

typedef struct UnkStruct_0236a444 {
    u8 pad[0x54];
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 pad60[4];
    s32 unk64;
    u8 pad68[0x78 - 0x68];
    u16 unk78;
    u8 pad7a[0x84 - 0x7a];
    s32 unk84;
    s32 unk88;
    s32 unk8C;
    s32 unk90;
    s32 unk94;
    s32 unk98;
    u8 pad9c[4];
    s32 unkA0;
    u8 padA4[0xb4 - 0xa4];
    u16 unkB4;
} UnkStruct_0236a444;

typedef struct UnkStruct_022c8f24 UnkStruct_022c8f24;
struct UnkStruct_022c8f24 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022c8f24 *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
    u16 unk20E;
    u8 pad1b[0x218 - 0x210];
    u16 unk218;
    u8 pad2[0x220 - 0x21a];
    s32 unk220;
    u8 pad3[4];
    u8 unk228[4];
};

extern UnkStruct_022c8f24_2 *data_ov000_02369acc;
extern UnkStruct_022c8f24_4 data_022c4560;
extern UnkStruct_022c8f24_5 data_ov000_02369ac0;
extern s16 data_ov000_02362d04[][8];
extern s16 data_ov000_02362ce4[][8];
extern s32 data_ov000_0236ac60;
extern UnkStruct_0236a444 data_ov000_0236a444;

BOOL func_ov000_022cc6d4(void);
void func_ov000_022cc00c(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022c8e64(UnkStruct_022c8f24 *s);

void func_ov000_022c8f24(UnkStruct_022c8f24 *s) {
    UnkStruct_022c8f24_3 *p = data_ov000_02369acc->unk214;
    s32 v;
    BOOL b;
    s32 i;
    s32 t;
    s32 z;
    if (func_ov000_022cc6d4()) {
        b = TRUE;
        if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
            b = FALSE;
        if (b)
            i = 1;
        else
            i = data_022c4560.unk14;
        v = data_ov000_02362d04[i][data_ov000_02369ac0.unk8 - 1];
    } else {
        b = TRUE;
        if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
            b = FALSE;
        if (b)
            i = 1;
        else
            i = data_022c4560.unk14;
        v = data_ov000_02362ce4[i][data_ov000_02369ac0.unk8 - 1];
    }
    func_ov000_022cc00c(s->unk228, s->unk220, 0x16, v, 0);
    func_ov000_022cc00c(p->unk30C, p->unk304, 9, v, 0);
    p->unk216 |= 4;
    p->unk29A |= 4;
    p->unk26E &= ~4;
    t = data_ov000_0236ac60;
    z = 0;
    data_ov000_0236a444.unkA0 = -0x1c0000;
    data_ov000_0236a444.unk84 = t;
    data_ov000_0236a444.unk88 = z;
    data_ov000_0236a444.unk8C = t;
    data_ov000_0236a444.unk90 = z;
    data_ov000_0236a444.unk94 = 0x28000;
    data_ov000_0236a444.unk98 = z;
    data_ov000_0236a444.unkB4 = 0x200;
    data_ov000_0236a444.unk64 = -0x1d8000;
    data_ov000_0236a444.unk54 = z;
    data_ov000_0236a444.unk58 = 0x28000;
    data_ov000_0236a444.unk5C = 0xa0000;
    data_ov000_0236a444.unk78 = 0x100;
    s->unk20C = 0x80;
    s->unk20E = 0;
    s->unk218 = 0;
    s->func = func_ov000_022c8e64;
}
