#include <nitro/types.h>

typedef struct {
    void (*state)(void *);
    u8 pad4[0x70];
    s32 count;
    s32 limit;
    u8 pad7c[2];
    u16 timer;
    u16 period;
} UnkWork_020625fc;

void TalkDemo_SetFlags(s32 a, s32 b);
void TalkDemoScene_State_StartPage(void *w);

#pragma thumb on
void TalkDemoScene_State_RockPageNext(UnkWork_020625fc *w) {
    w->timer++;
    if (w->timer < w->period)
        return;
    w->count++;
    if (w->count >= w->limit) {
        TalkDemo_SetFlags(3, 0x100);
        w->state = NULL;
        return;
    }
    w->state = TalkDemoScene_State_StartPage;
}
#pragma thumb reset
