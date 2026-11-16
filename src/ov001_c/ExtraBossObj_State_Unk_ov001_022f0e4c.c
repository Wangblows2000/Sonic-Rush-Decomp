#include <nitro/types.h>

typedef struct UnkStruct_022f0e4c_o {
    u8 pad0[0xe4];
    s32 *unkE4;
    u8 padE8[0x10c - 0xe8];
    u8 unk10C;
    u8 pad10D[3];
    u8 unk110;
    u8 pad111[3];
    s32 unk114;
    s32 unk118;
} UnkStruct_022f0e4c_o;

typedef struct UnkStruct_022f0e4c_q {
    u8 pad0[0x14];
    s16 unk14;
    u8 pad16[0x13c - 0x16];
    s32 unk13C;
    s32 unk140;
    s32 unk144;
} UnkStruct_022f0e4c_q;

typedef struct UnkStruct_022f0e4c {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x74 - 0x24];
    void (*unk74)(void);
    u8 pad78[0x7c - 0x78];
    struct UnkStruct_022f0e4c *unk7C;
    u8 pad80[0x84 - 0x80];
    UnkStruct_022f0e4c_o *unk84;
    u8 pad88[0x160 - 0x88];
    u32 unk160;
    u8 pad164[0x1cc - 0x164];
    s32 unk1CC;
    s32 unk1D0;
    s32 unk1D4;
    s16 unk1D8;
    u8 pad1DA[0x1dc - 0x1da];
    UnkStruct_022f0e4c_q *unk1DC;
} UnkStruct_022f0e4c;

extern u16 data_ov001_02316840[];
extern void *data_ov001_0231f068;
extern void *data_ov001_0231f074;

void ExtraBossObj_State_Unk_ov001_022f0cbc(void);
void ExtraBossObj_State_Unk_ov001_022f2ca4(void);
void ExtraBossObj_State_Unk_ov001_022f0b70(void);
void Model_SetAnimation(UnkStruct_022f0e4c_o *o, u32 slot, void *res, u32 idx, void *tex);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void ExtraBossObj_State_Unk_ov001_022f0e4c(UnkStruct_022f0e4c *p) {
    UnkStruct_022f0e4c *o = p->unk7C;
    UnkStruct_022f0e4c_q *q = o->unk1DC;
    UnkStruct_022f0e4c_o *a;
    UnkStruct_022f0e4c_o *b;
    s32 x;
    s32 c;
    s32 i;
    s32 d;
    s32 y;
    s32 j;

    p->unk1D8 -= 0x5b;
    if (p->unk1D8 <= 0) {
        p->unk1D8 = 0;
        p->unk18 = q->unk13C;
        p->unk1C = -q->unk140;
        p->unk20 = q->unk144;
        q->unk14 = 0;
        a = o->unk84;
        b = p->unk84;
        if (*a->unkE4 > 0xb4000) {
            o->unk1CC = -0x4000;
            o->unk74 = ExtraBossObj_State_Unk_ov001_022f0cbc;
            p->unk74 = ExtraBossObj_State_Unk_ov001_022f2ca4;
            return;
        }
        if (*a->unkE4 < 0xb4000) {
            o->unk1CC = 0x4000;
            o->unk74 = ExtraBossObj_State_Unk_ov001_022f0cbc;
            p->unk74 = ExtraBossObj_State_Unk_ov001_022f2ca4;
            return;
        }
        Model_SetAnimation(a, 0, data_ov001_0231f068, data_ov001_02316840[0x3c], 0);
        Model_SetAnimation(a, 4, data_ov001_0231f074, data_ov001_02316840[0x3d], 0);
        a->unk10C &= ~4;
        a->unk110 &= ~4;
        Model_SetAnimation(b, 0, data_ov001_0231f068, data_ov001_02316840[0x3e], 0);
        Model_SetAnimation(b, 4, data_ov001_0231f074, data_ov001_02316840[0x3f], 0);
        b->unk10C &= ~4;
        b->unk110 &= ~4;
        b->unk118 = 0x1000;
        b->unk114 = 0x1000;
        a->unk114 = b->unk114;
        o->unk74 = ExtraBossObj_State_Unk_ov001_022f0b70;
        p->unk74 = ExtraBossObj_State_Unk_ov001_022f2ca4;
        o->unk160 &= ~0x200;
    } else {
        x = p->unk1D0;
        c = q->unk13C;
        i = 2;
        do {
            x = c + FX_Mul(x - c, p->unk1D8);
        } while (i-- != 0);
        p->unk18 = x;
        y = p->unk1D4;
        d = -q->unk140;
        j = 2;
        do {
            y = d + FX_Mul(y - d, p->unk1D8);
        } while (j-- != 0);
        p->unk1C = y;
    }
}
