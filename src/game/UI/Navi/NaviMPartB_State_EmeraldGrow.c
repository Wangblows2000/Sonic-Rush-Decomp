#include <nitro/types.h>

typedef struct {
    u8 *res;
    u8 pad4[4];
    s16 t;
} UnkWork_020639e4;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_020639e4 *work;
} UnkTask_020639e4;

extern UnkTask_020639e4 *gTaskCurrent;

void AnimatorSprite_DrawRotScale(void *spr, s32 sx, s32 sy, s32 a);
void NaviMPartB_Draw(UnkWork_020639e4 *w);
void AnimatorSprite_SetAnimation(void *p, u16 a);
void NaviMPartB_State_EmeraldAnim(void);

#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Ease_020639e4(s32 from, s32 to, s16 t) {
    s32 i = 3;
    do {
        from += FX_Mul(to - from, t);
    } while (i-- != 0);
    return from;
}

void NaviMPartB_State_EmeraldGrow(void) {
    u8 *res;
    UnkWork_020639e4 *w;
    void *spr;
    s32 sx;
    s32 sy;

    w = gTaskCurrent->work;
    res = w->res;
    w->t += 0x400;
    spr = res + 0x640;
    sx = Ease_020639e4(0xf33, 0x1400, CLAMP(w->t, 0, 0x1000));
    sy = Ease_020639e4(0xf33, 0x1400, CLAMP(w->t, 0, 0x1000));
    AnimatorSprite_DrawRotScale(spr, sx, sy, 0);
    NaviMPartB_Draw(w);
    if (w->t < 0x1000)
        return;
    gTaskCurrent->fn = NaviMPartB_State_EmeraldAnim;
    AnimatorSprite_SetAnimation(res + 0x688, *(u16 *)(res + 0x690));
}
