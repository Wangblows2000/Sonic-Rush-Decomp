#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    u8 pad4[4];
    s32 unk8;
    s16 unkC;
} UnkObj_020472bc;

void func_020472a8(void);

#pragma thumb on
static inline s32 FX_Mul_020472bc(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Lerp_020472bc(s32 v, s32 to, s32 t, s32 n) {
    do {
        v += FX_Mul_020472bc(to - v, t);
    } while (n--);
    return v;
}

void func_020472bc(void *w, UnkObj_020472bc *o) {
    o->unkC += 40.96f;
    if (o->unkC >= 0x1000) {
        o->unkC = 0x1000;
        o->fn = func_020472a8;
    }
    o->unk8 = Lerp_020472bc(-0x23000, -0xa0000, o->unkC, 2);
}
#pragma thumb reset
