#include <nitro/types.h>

typedef struct {
    u8 w0x2;
    u8 w0x1;
    u8 w1x2;
    u8 w1x1;
} UnkWindow_0205feb8;

typedef struct {
    void (*state)(void *);
    u8 pad4[4];
    u16 unk8;
} UnkTask_0205feb8;

extern UnkWindow_0205feb8 data_0208923c;

void TalkDemoWindow_StartRaster(UnkTask_0205feb8 *t);
void TalkDemoWindow_State_OpenCenter(void *t);

#pragma thumb on
void TalkDemoWindow_State_BeginOpenCenter(UnkTask_0205feb8 *t) {
    UnkWindow_0205feb8 *w;

    TalkDemoWindow_StartRaster(t);
    w = (void *)&data_0208923c;
    w->w0x1 = 1;
    w->w0x2 = 0;
    w->w1x1 = 0;
    w->w1x2 = 1;
    t->unk8 = 0;
    t->state = (void (*)(void *))TalkDemoWindow_State_OpenCenter;
}
#pragma thumb reset
