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

void func_ov001_022cf90c(UnkWork_022cf6e8 *w);
void func_ov001_022cf8b4(UnkWork_022cf6e8 *w);
void func_ov001_022cf688(void);

#pragma thumb on
void func_ov001_022cf6e8(void) {
    UnkWork_022cf6e8 *w;

    w = gTaskCurrent->work;
    w->timer++;
    if (w->timer >= 2) {
        func_ov001_022cf90c(w);
        func_ov001_022cf8b4(w);
        w->timer = 0;
        gTaskCurrent->func = func_ov001_022cf688;
    }
}
#pragma thumb reset
