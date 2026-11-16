#include <nitro/types.h>

typedef struct UnkStruct_02302658_d {
    u32 unk0;
    s32 unk4;
} UnkStruct_02302658_d;

typedef struct UnkStruct_02302658_o {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x10];
    s32 unk30;
    s32 unk34;
    u8 pad38[0x18];
    s32 unk50;
    s32 unk54;
    u8 pad58[4];
    s32 unk5C;
    s32 unk60;
    u8 pad64[0x1dc - 0x64];
    UnkStruct_02302658_d *unk1DC;
} UnkStruct_02302658_o;

typedef struct UnkStruct_02302658_q {
    u32 unk0;
    u8 pad4[0x4c];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 pad60[0x310 - 0x60];
    s32 unk310;
} UnkStruct_02302658_q;

extern u32 gRandSeed;

UnkStruct_02302658_q *ExtraBoss_GetWork(void);
UnkStruct_02302658_o *func_ov001_02300c94(void);
s32 func_0200203c(s32 n, s32 d);
s32 func_0200225c(s32 n, s32 d);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Abs(s32 x) {
    return x < 0 ? -x : x;
}

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov001_02302658(void) {
    UnkStruct_02302658_q *q;
    UnkStruct_02302658_o *o;
    UnkStruct_02302658_d *d;
    s32 dx;
    s32 dy;
    s32 adx;
    s32 ady;
    s32 sx;
    s32 sy;

    q = ExtraBoss_GetWork();
    o = func_ov001_02300c94();
    d = o->unk1DC;
    if (d->unk0 & 1) {
        if (o->unk18 < q->unk54 + 0x10000)
            o->unk18 = q->unk54 + 0x10000;
        else if (o->unk18 > q->unk58 - 0x10000)
            o->unk18 = q->unk58 - 0x10000;
        if (o->unk1C < q->unk50 + 0x10000)
            o->unk1C = q->unk50 + 0x10000;
        else if (o->unk1C > q->unk5C)
            o->unk1C = q->unk5C;
    }
    if (d->unk4 == 6)
        return;
    if (d->unk0 & 4) {
        o->unk50 = o->unk54 = o->unk5C = o->unk60 = 0;
        o->unk30 = (func_0200203c(Rand(), 5) - 2) << 12;
        o->unk34 = (func_0200203c(Rand(), 5) - 2) << 12;
    }
    if (ExtraBoss_GetWork()->unk0 & 0x40) {
        dx = o->unk18 - 0xc0000;
        q = ExtraBoss_GetWork();
        dy = o->unk1C - q->unk310;
        adx = Abs(dx);
        ady = Abs(dy);
        sx = func_0200225c(FX_Mul(adx, 0x3000) + FX_Mul(0xc0000 - adx, 0x5000), 0xc0000);
        sy = func_0200225c(FX_Mul(ady, 0x6000) + FX_Mul(0x180000 - ady, 0x6800), 0x180000);
        if (dx < 0) {
            if (adx < sx)
                o->unk18 = 0xc0000;
            else
                o->unk18 += sx;
        } else if (dx > 0) {
            if (adx < sx)
                o->unk18 = 0xc0000;
            else
                o->unk18 -= sx;
        }
        if (dy < 0) {
            if (ady >= sy) {
                o->unk1C += sy;
                return;
            }
            o->unk1C = ExtraBoss_GetWork()->unk310;
            d->unk0 |= 4;
        } else if (dy > 0) {
            if (ady >= sy) {
                o->unk1C -= sy;
                return;
            }
            o->unk1C = ExtraBoss_GetWork()->unk310;
            d->unk0 |= 4;
        }
    } else {
        d->unk0 &= ~4;
    }
}
