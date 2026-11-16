#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    u8 pad4[0xc];
    s32 unk10;
    u8 pad14[0x50];
    s16 t;  /* 0x64 */
} UnkObj_02046f8c;

void func_02046f78(void);

#pragma thumb on
static inline s32 FX_Mul_02046f8c(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Lerp_02046f8c(s32 v, s32 to, s32 t, s32 n) {
    do {
        v += FX_Mul_02046f8c(to - v, t);
    } while (n--);
    return v;
}

void func_02046f8c(void *w, UnkObj_02046f8c *o) {
    o->t += 0x333;
    if (o->t >= 0x1000) {
        o->t = 0x1000;
        o->fn = func_02046f78;
    }
    o->unk10 = Lerp_02046f8c(0xe0000, 0xb4000, o->t, 3);
}
#pragma thumb reset
