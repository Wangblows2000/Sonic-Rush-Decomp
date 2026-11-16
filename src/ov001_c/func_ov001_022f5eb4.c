#include <nitro/types.h>

typedef struct UnkStruct_022f5eb4_o {
    u8 pad0[0xe4];
    s32 *unkE4;
    u8 padE8[0x10c - 0xe8];
    u8 unk10C;
    u8 pad10D[7];
    s32 unk114;
} UnkStruct_022f5eb4_o;

typedef struct UnkStruct_022f5eb4_q {
    u8 pad0[0x134];
    s32 unk134;
    s32 unk138;
} UnkStruct_022f5eb4_q;

typedef struct UnkStruct_022f5eb4_g {
    u8 pad0[0x308];
    s32 unk308;
    s32 unk30C;
} UnkStruct_022f5eb4_g;

typedef struct UnkStruct_022f5eb4 {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x74 - 0x20];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f5eb4_o *unk84;
    u8 pad88[0x1c8 - 0x88];
    s16 unk1C8;
    u8 pad1CA[0x1d0 - 0x1ca];
    s32 unk1D0;
    s16 unk1D4;
    u8 pad1D6[0x1dc - 0x1d6];
    UnkStruct_022f5eb4_q *unk1DC;
} UnkStruct_022f5eb4;

extern void *data_ov001_0231f068;
extern s32 data_ov001_0231f520;
extern s32 data_ov001_0231f524;

UnkStruct_022f5eb4 *func_ov001_02300c94(void);
void func_ov001_022f5da0(void);
void func_ov001_022f5a24(void);
void Model_SetAnimation(UnkStruct_022f5eb4_o *o, u32 slot, void *res, u32 idx, void *tex);
u16 func_020027a4(s32 y, s32 x);
void func_ov001_022fc09c(void *parent, s32 x, s32 y, s32 z, u16 ang);
s32 func_02002094(s32 n, s32 d);
UnkStruct_022f5eb4_g *func_ov001_022ffce8(void);

void func_ov001_022f5eb4(UnkStruct_022f5eb4 *p) {
    UnkStruct_022f5eb4_o *a = p->unk84;
    UnkStruct_022f5eb4 *t;
    UnkStruct_022f5eb4_q *q;
    u16 ang;

    if (!(a->unk10C & 2))
        return;
    t = func_ov001_02300c94();
    p->unk74 = func_ov001_022f5da0;
    Model_SetAnimation(a, 0, data_ov001_0231f068, 6, 0);
    a->unk10C &= ~4;
    *a->unkE4 = 0x64000;
    ang = func_020027a4(t->unk1C - p->unk1C, t->unk18 - p->unk18);
    if (ang == 0x8000) {
        q = p->unk1DC;
        p->unk74 = func_ov001_022f5a24;
        a->unk114 = 0;
        p->unk1D0 = 0x64000;
        func_ov001_022fc09c(p, q->unk134 - data_ov001_0231f524, -(q->unk138 + data_ov001_0231f520), 0, (((p->unk1D0 >> 13) << 16) / 360) - 0xa38f);
        p->unk1C8 = 0;
        p->unk1D4 = 0;
        return;
    }
    if (ang < 0x5c71)
        ang = 0x5c71;
    else if (ang > 0xb1c7)
        ang = 0xb1c7;
    p->unk1D0 = func_02002094((u16)(ang + 0xa38f), 0xb6) << 13;
    a->unk114 = func_02002094(func_ov001_022ffce8()->unk308 * 0xa000 + ((func_ov001_022ffce8()->unk30C - func_ov001_022ffce8()->unk308) << 16), func_ov001_022ffce8()->unk30C);
    if (p->unk1D0 < 0x64000)
        a->unk114 = -a->unk114;
}
