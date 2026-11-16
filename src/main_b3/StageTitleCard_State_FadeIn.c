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

extern UnkDisp_0205b2d0 gDisplaySub;
extern UnkDisp_0205b2d0 gDisplayMain;

void StageTitleCard_State_BeginShow(void);

static inline s32 FX_Mul_0205b2d0(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void StageTitleCard_State_FadeIn(UnkWork_0205b2d0 *w) {
    w->t += 0xa3;
    if (w->t >= 0x1000) {
        w->t = 0x1000;
        w->fn = StageTitleCard_State_BeginShow;
    }
    gDisplaySub.brightness = (FX_Mul_0205b2d0(-0x10000, w->t) + 0x10000) >> 12;
    gDisplayMain.brightness = gDisplaySub.brightness;
}
