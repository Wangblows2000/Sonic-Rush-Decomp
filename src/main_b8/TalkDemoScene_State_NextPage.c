#include <nitro/types.h>

typedef struct {
    void (*state)(void *);
    u8 pad4[0xc];
    s32 unk10;
    u8 pad14[0x60];
    s32 count;
    s32 limit;
    u8 pad7c[2];
    u16 timer;
    u16 period;
} UnkWork_02062738;

void TalkDemo_SetFlags(s32 a, s32 b);
void TalkDemoScene_StopPageSound(UnkWork_02062738 *w);
void TalkDemoScene_State_StartPage(void *w);

#pragma thumb on
void TalkDemoScene_State_NextPage(UnkWork_02062738 *w) {
    w->timer++;
    if (w->timer < w->period)
        return;
    w->count++;
    if (w->count >= w->limit) {
        TalkDemo_SetFlags(3, 0x100);
        w->unk10 = 0xc;
        w->state = NULL;
    } else {
        w->unk10 = 0xb;
        w->state = TalkDemoScene_State_StartPage;
    }
    TalkDemoScene_StopPageSound(w);
}
#pragma thumb reset
