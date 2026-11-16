#include <nitro/types.h>

typedef struct UnkStruct_022f1c0c_o {
    u8 pad0[0x74];
    void (*state)(struct UnkStruct_022f1c0c_o *p);
    u8 pad78[0xf0 - 0x78];
    u32 flagsF0;
    u8 padF4[0x128 - 0xf4];
    u32 flags128;
} UnkStruct_022f1c0c_o;

typedef struct UnkStruct_022f1c0c_w {
    u8 pad0[0x18];
    UnkStruct_022f1c0c_o *unk18;
} UnkStruct_022f1c0c_w;

typedef struct UnkStruct_022f1c0c {
    u8 pad0[0x18];
    s32 x;
    s32 y;
    u8 pad20[0x50 - 0x20];
    s32 vx;
    s32 vy;
    u8 pad58[0x74 - 0x58];
    void (*state)(struct UnkStruct_022f1c0c *p);
    u8 pad78[0xb8 - 0x78];
    u32 flagsB8;
    u8 padBC[0xf0 - 0xbc];
    u32 flagsF0;
    u8 padF4[0x128 - 0xf4];
    u32 flags128;
    u8 pad12C[0x160 - 0x12c];
    u32 flags160;
    u8 pad164[0x1dc - 0x164];
    UnkStruct_022f1c0c_w *work;
} UnkStruct_022f1c0c;

extern s16 data_0206e470[];

void func_ov001_022f1a90(UnkStruct_022f1c0c *p);
void func_ov001_022f2ca4(UnkStruct_022f1c0c_o *p);
u16 FX_Atan2(s32 y, s32 x);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_022f1c0c(UnkStruct_022f1c0c *p) {
    UnkStruct_022f1c0c_o *o = p->work->unk18;
    u16 a;

    p->state = func_ov001_022f1a90;
    o->state = func_ov001_022f2ca4;
    p->flagsB8 &= ~0x200;
    p->flagsF0 &= ~0x200;
    p->flags128 |= 0x200;
    p->flags160 |= 0x200;
    o->flagsF0 |= 0x200;
    o->flags128 |= 0x200;
    a = FX_Atan2(0xc0000 - p->y, 0x162000 - p->x);
    p->vx = FX_Mul(COS(a), 0x5000);
    p->vy = FX_Mul(SIN(a), 0x5000);
}
