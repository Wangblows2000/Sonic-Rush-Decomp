#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    u8 pad4[0x90];
    s32 unk94;
    s32 unk98;
    s16 unk9C;
    s16 t;  /* 0x9e */
} UnkObj_0205c3a0;

void StageTitleCardPartE0_State_Done(void);

static inline s32 FX_Mul_0205c3a0(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void StageTitleCardPartE0_State_Enter(void *w, UnkObj_0205c3a0 *o) {
    o->t += 0x28;
    if (o->t >= 0x1000) {
        o->t = 0x1000;
        o->fn = StageTitleCardPartE0_State_Done;
    }
    o->unk94 = 0x1a0000 + FX_Mul_0205c3a0(-0xc8000, o->t);
    o->unk98 = -0x5c000 + FX_Mul_0205c3a0(0xc8000, o->t);
    o->unk9C = FX_Mul_0205c3a0(-0x40000000, o->t) >> 12;
}
