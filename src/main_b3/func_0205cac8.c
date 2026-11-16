#include <nitro/types.h>

typedef struct { u8 pad0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
typedef struct {
    void (*fn)(void);
    u8 pad4[8];
    s16 t;  /* 0xc */
} UnkObj_0205cac8;
typedef struct {
    u8 pad0[0x4b4];
    s32 v[6]; /* 0x4b4 */
} UnkWork_0205cac8;

void func_0205caa0(void);

static inline s32 FX_Mul_0205cac8(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Lerp_0205cac8(s32 v, s32 to, s32 t, s32 n) {
    do {
        v += FX_Mul_0205cac8(to - v, t);
    } while (n--);
    return v;
}

void func_0205cac8(UnkWork_0205cac8 *w, UnkObj_0205cac8 *o) {
    o->t += 0x28;
    if (o->t >= 0x1000) {
        o->t = 0x1000;
        o->fn = func_0205caa0;
    }
    if (gGameState.unk1C != 8 || gGameState.unk1D != 2) {
        w->v[0] = Lerp_0205cac8(0x9e000, 0x80000, o->t, 0);
        w->v[1] = Lerp_0205cac8(-0x5b000, -0x5b000, o->t, 0);
        w->v[2] = Lerp_0205cac8(0x1e000, 0x3c000, o->t, 0);
        w->v[3] = Lerp_0205cac8(0x80000, 0x80000, o->t, 0);
        w->v[4] = Lerp_0205cac8(-0x5b000, -0x5b000, o->t, 0);
        w->v[5] = Lerp_0205cac8(0, 0, o->t, 0);
    } else {
        w->v[0] = Lerp_0205cac8(0x80000, 0x80000, o->t, 0);
        w->v[1] = Lerp_0205cac8(-0x5b000, -0x5b000, o->t, 0);
        w->v[2] = Lerp_0205cac8(0x1e000, 0x3c000, o->t, 0);
        w->v[3] = Lerp_0205cac8(0x80000, 0x80000, o->t, 0);
        w->v[4] = Lerp_0205cac8(-0x5b000, -0x5b000, o->t, 0);
        w->v[5] = Lerp_0205cac8(0, 0, o->t, 0);
    }
}
