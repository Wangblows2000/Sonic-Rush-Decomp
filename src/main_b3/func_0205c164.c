#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    u8 pad4[0x48];
    s32 unk4C;
    s32 unk50;
    u8 pad54[0x10];
    s16 unk64;
    s16 unk66;
    s32 unk68;
    u16 delay;  /* 0x6c */
    s16 t;      /* 0x6e */
} UnkObj_0205c164;
typedef struct { u8 pad0[0x1c]; u8 unk1C; } UnkStruct_022c4560;
typedef struct { s32 a; s32 b; } UnkEnt_0205c164;
extern UnkStruct_022c4560 gGameState;
extern UnkEnt_0205c164 data_02076bb4[];
extern UnkEnt_0205c164 data_02076bb8[];

void func_0205c140(void);

static inline s32 FX_Mul_0205c164(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Lerp_0205c164(s32 v, s32 to, s32 t, s32 n) {
    do {
        v += FX_Mul_0205c164(to - v, t);
    } while (n--);
    return v;
}

static inline s32 Lerp2_0205c164(s32 v, s32 base, s32 t, s32 n) {
    do {
        v = FX_Mul_0205c164(v - base, t) + base;
    } while (n--);
    return v;
}

void func_0205c164(void *w, UnkObj_0205c164 *o) {
    if (o->delay == 0) {
        o->t += 0x51;
        if (o->t >= 0x1000) {
            o->t = 0x1000;
            o->fn = func_0205c140;
        }
    } else {
        o->delay--;
    }
    o->unk4C = Lerp_0205c164(-0x50000, data_02076bb4[gGameState.unk1C].a, o->t, 3);
    o->unk50 = o->unk68 + Lerp_0205c164(data_02076bb8[gGameState.unk1C].a, data_02076bb8[gGameState.unk1C].a, o->t, 3);
    o->unk64 = Lerp_0205c164(0x4000, 0x1000, o->t, 1);
    o->unk66 = Lerp2_0205c164(0x1000, 0x1000, o->t, 0);
}
