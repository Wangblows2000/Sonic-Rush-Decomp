#include <nitro/types.h>

typedef struct {
    u8 *res;
    u8 pad4[4];
    s16 t;
    u8 pada[2];
    s32 unkc;
    s32 unk10;
} UnkWork_02063b3c;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_02063b3c *work;
} UnkTask_02063b3c;

extern UnkTask_02063b3c *gTaskCurrent;

void AnimatorSprite_DrawRotScale(void *spr, s32 sx, s32 sy, s32 a);
void func_02063890(UnkWork_02063b3c *w);
void func_020639e4(void);

#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Ease_02063b3c(s32 from, s32 to, s16 t) {
    s32 i;
    s32 v;
    i = 3;
    v = from;
    do {
        v += FX_Mul(to - v, t);
    } while (i-- != 0);
    return v;
}

void func_02063b3c(void) {
    u8 *res;
    UnkWork_02063b3c *w;
    void *spr;
    s32 sx;
    s32 sy;

    w = gTaskCurrent->work;
    res = w->res;
    w->t += 0x100;
    spr = res + 0x640;
    sx = Ease_02063b3c(0x1400, 0xf33, CLAMP(w->t, 0, 0x1000));
    sy = Ease_02063b3c(0x1400, 0xf33, CLAMP(w->t, 0, 0x1000));
    if (sx <= 0x1000)
        w->unkc = w->unk10;
    AnimatorSprite_DrawRotScale(spr, sx, sy, 0);
    func_02063890(w);
    if (w->t < 0x1000)
        return;
    gTaskCurrent->fn = func_020639e4;
    w->t = 0;
}
