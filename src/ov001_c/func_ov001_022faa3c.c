#include <nitro/types.h>

typedef struct UnkStruct_022faa3c {
    u8 pad0[0x14];
    u32 flags;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x3c - 0x24];
    s16 unk3C;
    s16 unk3E;
    u8 pad40[0x50 - 0x40];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    u8 pad5C[0x64 - 0x5c];
    s32 unk64;
    u8 pad68[0x1d0 - 0x68];
    s32 unk1D0;
} UnkStruct_022faa3c;

extern s16 data_0206e470[];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

void func_ov001_022fc934(s32 a, s32 b, s32 c, s32 d, BOOL flag);
void func_02036754(u32 *p, u32 a, u32 b);
UnkStruct_022faa3c *func_ov001_02300c94(void);
u16 func_020027a4(s32 y, s32 x);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_022faa3c(UnkStruct_022faa3c *p) {
    UnkStruct_022faa3c *t;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 idx;
    s32 vx;
    s32 vy;

    if (p->unk20 >= 0) {
        p->flags |= 8;
        func_ov001_022fc934(0, p->unk18, p->unk1C, 0, 1);
        func_02036754(0, 0, 0xb1);
        return;
    }
    if (p->unk58 + p->unk64 >= 0x4000) {
        p->unk58 = 0x4000;
        p->unk64 = 0;
    }
    t = func_ov001_02300c94();
    dx = t->unk18 - p->unk18;
    dy = t->unk1C - p->unk1C;
    idx = (s32)(func_020027a4(dy, dx) & 0xffff) >> 4;
    vx = FX_Mul(p->unk1D0, COS_IDX(idx));
    vy = FX_Mul(p->unk1D0, SIN_IDX(idx));
    if ((vx > 0 && p->unk50 < -vx) || (vx < 0 && p->unk50 > -vx))
        vx = 0;
    if ((vy > 0 && p->unk54 < -vy) || (vy < 0 && p->unk54 > -vy))
        vy = 0;
    p->unk50 = vx;
    p->unk54 = vy;
    if (p->unk50 >= 0 && p->unk50 > dx)
        p->unk50 = dx;
    else if (p->unk50 < 0 && p->unk50 < dx)
        p->unk50 = dx;
    if (p->unk54 >= 0 && p->unk54 > dy)
        p->unk54 = dy;
    else if (p->unk54 < 0 && p->unk54 < dy)
        p->unk54 = dy;
    dz = t->unk20 - p->unk20;
    p->unk3C = func_020027a4(dy, dz);
    p->unk3E = func_020027a4(dx, dz);
    p->unk3C >>= 1;
    p->unk3E >>= 1;
}
