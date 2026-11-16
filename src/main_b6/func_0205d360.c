#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u8 x4[4];
    s16 h8;
    s16 ha;
    u8 padc[0x40];
    s32 x4c;
    s32 x50;
    u8 pad54[8];
    s32 x5c;
    s32 x60;
    s16 h64;
    s16 h66;
    u8 pad68[8];
} UnkSpr_0205d360;

void func_02032480(void *p, s32 a, s32 b, s32 c);

static inline s32 FX_Mul_0205d360(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_0205d360(u8 *w) {
    u32 i;
    UnkSpr_0205d360 *s = (UnkSpr_0205d360 *)(w + 0x180);

    for (i = 0; i < 2; i++) {
        s->h8 = (s->x4c + s->x5c - FX_Mul_0205d360(s->x5c, s->h64)) >> 12;
        s->ha = (s->x50 + s->x60 - FX_Mul_0205d360(s->x60, s->h66)) >> 12;
        func_02032480(s->x4, s->h64, s->h66, 0);
        s++;
    }
}
