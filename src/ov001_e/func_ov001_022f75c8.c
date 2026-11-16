#include <nitro/types.h>

typedef struct UnkStruct_022f75c8_o {
    u8 pad0[4];
    s16 x;
    s16 y;
} UnkStruct_022f75c8_o;

typedef struct UnkStruct_022f75c8 {
    u8 pad0[0xb10];
    UnkStruct_022f75c8_o objB10;
    u8 padB18[0xe38 - 0xb18];
    u32 flags;
    u8 padE3C[0xe9a - 0xe3c];
    s16 t;
    s32 unkE9C;
} UnkStruct_022f75c8;

typedef struct UnkStruct_022b4574 {
    u8 pad0[8];
    void (*func)(void);
} UnkStruct_022b4574;

extern UnkStruct_022f75c8 *data_ov001_0231f084;
extern UnkStruct_022b4574 *data_022b4574;

void func_ov001_022f741c(void);
void func_ov001_022f7448(void);
void func_02036754(s32 a, s32 b, s32 c);
void func_02032480(void *p, s32 x, s32 y, s32 z);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_022f75c8(void) {
    UnkStruct_022f75c8 *g;
    s32 i;
    s32 s;
    s32 t;

    if (data_ov001_0231f084->flags & 2) {
        data_022b4574->func = func_ov001_022f741c;
        func_ov001_022f741c();
        data_ov001_0231f084->flags &= ~0x10000;
        data_ov001_0231f084->flags |= 0x20000;
        return;
    }
    data_ov001_0231f084->t += 0x100;
    if (data_ov001_0231f084->t >= 0x1000) {
        data_022b4574->func = func_ov001_022f7448;
        data_ov001_0231f084->unkE9C = 0x10;
        func_02036754(0, 8, 3);
    }
    g = data_ov001_0231f084;
    i = 5;
    s = 0x1000;
    t = g->t;
    do {
        s = FX_Mul(s - 0x2000, t) + 0x2000;
    } while (i-- != 0);
    {
        UnkStruct_022f75c8_o *o = &g->objB10;
        o->x = 0x50;
        o->y = 0x8a;
        func_02032480(o, s, s, 0);
    }
}
