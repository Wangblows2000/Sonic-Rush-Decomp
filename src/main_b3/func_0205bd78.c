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
} UnkObj_0205bd78;

void func_0205bd4c(void);
typedef struct { u8 pad0[0x1c]; u8 unk1C; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;

static inline s32 FX_Mul_0205bd78(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Lerp_0205bd78(s32 v, s32 to, s32 t, s32 n) {
    do {
        v += FX_Mul_0205bd78(to - v, t);
    } while (n--);
    return v;
}

static inline s32 Lerp2_0205bd78(s32 v, s32 base, s32 t, s32 n) {
    do {
        v = FX_Mul_0205bd78(v - base, t) + base;
    } while (n--);
    return v;
}

void func_0205bd78(void *w, UnkObj_0205bd78 *o) {
    o->t += 0x51;
    if (o->t >= 0x1000) {
        o->t = 0x1000;
        o->fn = func_0205bd4c;
    }
    if (gGameState.unk1C < 7)
        o->unkA4 = Lerp_0205bd78(0x12c000, 0x78000, o->t, 3);
    else
        o->unkA4 = Lerp_0205bd78(0x12c000, 0x46000, o->t, 3);
    o->unkA8 = Lerp_0205bd78(0x91000, 0x91000, o->t, 3);
    o->unkBC = Lerp_0205bd78(0x2000, 0x1000, o->t, 1);
    o->unkBE = Lerp2_0205bd78(0x1000, 0x1000, o->t, 0);
}
