#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    u8 pad4[0x8];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s16 t;  /* 0x18 */
} UnkObj_0205b75c;

void StageTitleCardText3E8_State_Done(void);

static inline s32 FX_Mul_0205b75c(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Lerp_0205b75c(s32 v, s32 to, s32 t, s32 n) {
    do {
        v += FX_Mul_0205b75c(to - v, t);
    } while (n--);
    return v;
}

void StageTitleCardText3E8_State_Enter(void *w, UnkObj_0205b75c *o) {
    o->t += 0x51;
    if (o->t >= 0x1000) {
        o->t = 0x1000;
        o->fn = StageTitleCardText3E8_State_Done;
    }
    o->unkC = Lerp_0205b75c(0x118000, 0x50000, o->t, 3);
    o->unk10 = Lerp_0205b75c(0x28000, 0x28000, o->t, 3);
    o->unk14 = (s16)Lerp_0205b75c(0, 0x1000, o->t, 3);
}
