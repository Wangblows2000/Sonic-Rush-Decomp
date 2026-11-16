#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    u8 pad4[0xa0];
    s32 unkA4;
    s32 unkA8;
    s32 unkAC;
    s32 unkB0;
    u8 padB4[0xc];
    s16 t;  /* 0xc0 */
} UnkObj_0205b8fc;

void func_0205b8ec(void);

static inline s32 FX_Mul_0205b8fc(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_0205b8fc(void *w, UnkObj_0205b8fc *o) {
    o->t += 0x28;
    if (o->t >= 0x1000) {
        o->t = 0x1000;
        o->fn = func_0205b8ec;
    }
    o->unkA4 = o->unkAC + FX_Mul_0205b8fc(-0x44000 - o->unkAC, o->t);
    o->unkA8 = o->unkB0 + FX_Mul_0205b8fc(0x91000 - o->unkB0, o->t);
}
