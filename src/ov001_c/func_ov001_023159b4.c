#include <nitro/types.h>

typedef struct UnkStruct_023159b4_v {
    s32 x, y, z;
} UnkStruct_023159b4_v;

typedef struct UnkStruct_023159b4 {
    s32 state;
    s32 unk4;
    s32 unk8;
    float unkC;
    UnkStruct_023159b4_v pos;
    UnkStruct_023159b4_v tgt;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
} UnkStruct_023159b4;

typedef struct UnkStruct_023159b4_e {
    u8 pad0[0x20];
    UnkStruct_023159b4_v pos;
    UnkStruct_023159b4_v tgt;
    u8 pad38[0x58 - 0x38];
} UnkStruct_023159b4_e;

extern UnkStruct_023159b4 data_ov001_0231f6ac;
extern UnkStruct_023159b4_e *data_ov001_0231f6a4;
extern s32 data_ov001_0231ef88;
extern u16 data_ov001_0231ef84;

s32 func_ov001_0230c0a4(void);
void func_ov001_02315020(UnkStruct_023159b4 *c);
void VEC_Subtract(const UnkStruct_023159b4_v *a, const UnkStruct_023159b4_v *b, UnkStruct_023159b4_v *c);
void VEC_Add(const UnkStruct_023159b4_v *a, const UnkStruct_023159b4_v *b, UnkStruct_023159b4_v *c);
s32 VEC_Mag(const UnkStruct_023159b4_v *a);
s32 func_0200225c(s32 n, s32 d);
void func_ov001_02314914(s32 h, s32 a, s32 b);
void func_ov001_02314ae0(s32 h, UnkStruct_023159b4_v v);
void func_ov001_02314a98(s32 h, UnkStruct_023159b4_v v);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_023159b4(void) {
    UnkStruct_023159b4 *c = &data_ov001_0231f6ac;
    UnkStruct_023159b4_v dp;
    UnkStruct_023159b4_v dt;
    s32 lp;
    s32 lt;

    func_ov001_0230c0a4();
    if (c->unk4 != 0) {
        c->unk4 = 0;
        func_ov001_02315020(c);
    }
    VEC_Subtract(&c->pos, &data_ov001_0231f6a4[c->unk34].pos, &dp);
    VEC_Subtract(&c->tgt, &data_ov001_0231f6a4[c->unk34].tgt, &dt);
    lp = VEC_Mag(&dp);
    lt = VEC_Mag(&dt);
    if (c->state == 0xb) {
        c->unk38++;
        if (c->unk38 >= data_ov001_0231ef88) {
            c->unk28 = 0x400;
        } else {
            func_ov001_02314914(c->unk34, 0, data_ov001_0231ef84 / data_ov001_0231ef88);
            lp = 0;
        }
    } else {
        c->unk38 = 0;
    }
    if (c->unk8 == 0) {
        if (c->unk30 < lp && lp > c->unk2C) {
            if (lp > c->unk28) {
                dp.x = func_0200225c(dp.x, lp);
                dp.y = func_0200225c(dp.y, lp);
                dp.z = func_0200225c(dp.z, lp);
                dp.x = FX_Mul(dp.x, c->unk28);
                dp.y = FX_Mul(dp.y, c->unk28);
                dp.z = FX_Mul(dp.z, c->unk28);
            }
            VEC_Add(&dp, &data_ov001_0231f6a4[c->unk34].pos, &dp);
            func_ov001_02314ae0(c->unk34, dp);
        }
        if (c->unk30 < lt && lt > c->unk2C) {
            if (lt > c->unk28) {
                dt.x = func_0200225c(dt.x, lt);
                dt.y = func_0200225c(dt.y, lt);
                dt.z = func_0200225c(dt.z, lt);
                dt.x = FX_Mul(dt.x, c->unk28);
                dt.y = FX_Mul(dt.y, c->unk28);
                dt.z = FX_Mul(dt.z, c->unk28);
            }
            VEC_Add(&dt, &data_ov001_0231f6a4[c->unk34].tgt, &dt);
            func_ov001_02314a98(c->unk34, dt);
        }
    } else if (c->unk8 == 1) {
        dp.x = dp.x * c->unkC;
        dp.y = dp.y * c->unkC;
        dp.z = dp.z * c->unkC;
        VEC_Add(&dp, &data_ov001_0231f6a4[c->unk34].pos, &dp);
        func_ov001_02314ae0(c->unk34, dp);
        dt.x = dt.x * c->unkC;
        dt.y = dt.y * c->unkC;
        dt.z = dt.z * c->unkC;
        VEC_Add(&dt, &data_ov001_0231f6a4[c->unk34].tgt, &dt);
        func_ov001_02314a98(c->unk34, dt);
    }
}
