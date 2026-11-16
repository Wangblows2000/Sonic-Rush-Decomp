#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
    s16 brightness;
} UnkDisp_0205b2d0;

typedef struct {
    u8 pad0[0x474];
    void (*fn)(void);
    u8 pad478[0x8ce - 0x478];
    s16 t;  /* 0x8ce */
} UnkWork_0205b2d0;

extern UnkDisp_0205b2d0 data_02087d04;
extern UnkDisp_0205b2d0 data_0208922c;

void func_0205b2b4(void);

static inline s32 FX_Mul_0205b2d0(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_0205b2d0(UnkWork_0205b2d0 *w) {
    w->t += 0xa3;
    if (w->t >= 0x1000) {
        w->t = 0x1000;
        w->fn = func_0205b2b4;
    }
    data_02087d04.brightness = (FX_Mul_0205b2d0(-0x10000, w->t) + 0x10000) >> 12;
    data_0208922c.brightness = data_02087d04.brightness;
}
