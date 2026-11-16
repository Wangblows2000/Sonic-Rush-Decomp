#include <nitro/types.h>

typedef struct UnkStruct_02303e60_e {
    s32 x;
    s32 y;
    s32 z;
    s32 unkC;
    s32 vx;
    s32 vy;
    s32 unk18;
    s32 unk1C;
} UnkStruct_02303e60_e;

typedef struct UnkStruct_02303e60_g {
    u8 pad0[0x4c];
    s32 unk4C;
} UnkStruct_02303e60_g;

extern s16 data_0206e470[];

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

UnkStruct_02303e60_g *func_ov001_022ffce8(void);
s32 func_02002094(s32 n, s32 d);
UnkStruct_02303e60_e *func_ov001_02303468(void);
void func_ov001_0230328c(UnkStruct_02303e60_e *e);

void func_ov001_02303e60(s32 x, s32 y, s32 n) {
    UnkStruct_02303e60_g *g = func_ov001_022ffce8();
    UnkStruct_02303e60_e *e;
    s32 i;
    u16 ang;
    u16 step;

    if (g->unk4C < n)
        n = g->unk4C;
    if (n <= 0)
        return;
    g->unk4C -= n;
    if (n == 1) {
        ang = 0xc000;
        step = 0;
    } else {
        ang = 0xa000;
        step = func_02002094(0x4000, n - 1);
    }
    for (i = 0; i < n; i++, ang += step) {
        e = func_ov001_02303468();
        e->x = x;
        e->y = y;
        e->z = 0;
        e->unkC = 0x100;
        e->vx = FX_Mul(COS(ang), 0x2800);
        e->vy = FX_Mul(SIN(ang), 0x2800);
        e->unk18 = 0x11;
        e->unk1C = 5;
        func_ov001_0230328c(e);
    }
}
