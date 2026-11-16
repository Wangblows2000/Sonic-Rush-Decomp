#include <nitro/types.h>

typedef struct {
    s32 timer;
} UnkWork_022cf6e8;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022cf6e8 *work;
} UnkTask_022cf6e8;

extern UnkTask_022cf6e8 *gTaskCurrent;

void SegaLogo_LoadAssets(UnkWork_022cf6e8 *w);
void SegaLogo_ShowBackgrounds(UnkWork_022cf6e8 *w);
void SegaLogo_State_FadeIn(void);

#pragma thumb on
void SegaLogo_State_Init(void) {
    UnkWork_022cf6e8 *w;

    w = gTaskCurrent->work;
    w->timer++;
    if (w->timer >= 2) {
        SegaLogo_LoadAssets(w);
        SegaLogo_ShowBackgrounds(w);
        w->timer = 0;
        gTaskCurrent->func = SegaLogo_State_FadeIn;
    }
}
#pragma thumb reset
