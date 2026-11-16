#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 duration;
    s32 arg;
} UnkWork_020518dc;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_020518dc *work;
} UnkTask_020518dc;

extern s32 data_022c465c;
extern UnkTask_020518dc *data_022b4574;
void func_020371c8(UnkTask_020518dc *t);
void func_02052104(s32 a);

#pragma thumb on
void func_020518dc(void) {
    UnkWork_020518dc *w;
    if (data_022c465c == 0) {
        func_020371c8(data_022b4574);
        return;
    }
    w = data_022b4574->work;
    w->timer++;
    if (w->timer >= w->duration) {
        func_02052104(w->arg);
        func_020371c8(data_022b4574);
    }
}
#pragma thumb reset
