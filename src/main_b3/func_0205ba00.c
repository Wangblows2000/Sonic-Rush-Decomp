#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    u8 pad4[0xa0];
    s32 unkA4;
    s32 unkA8;
    u8 padAC[0x10];
    s16 unkBC;
    s16 unkBE;
    s16 t;  /* 0xc0 */
} UnkObj_0205ba00;

void func_0205b9d4(void);

static inline s32 FX_Mul_0205ba00(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Lerp_0205ba00(s32 v, s32 to, s32 t, s32 n) {
    do {
        v += FX_Mul_0205ba00(to - v, t);
    } while (n--);
    return v;
}

static inline s32 Lerp2_0205ba00(s32 v, s32 base, s32 t, s32 n) {
    do {
        v = FX_Mul_0205ba00(v - base, t) + base;
    } while (n--);
    return v;
}

void func_0205ba00(void *w, UnkObj_0205ba00 *o) {
    o->t += 0x51;
    if (o->t >= 0x1000) {
        o->t = 0x1000;
        o->fn = func_0205b9d4;
    }
    o->unkA4 = Lerp_0205ba00(0x168000, 0xb4000, o->t, 3);
    o->unkA8 = Lerp_0205ba00(0x91000, 0x91000, o->t, 3);
    o->unkBC = Lerp_0205ba00(0x2000, 0x1000, o->t, 1);
    o->unkBE = Lerp2_0205ba00(0x1000, 0x1000, o->t, 0);
}
