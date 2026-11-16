#include <nitro/types.h>

typedef struct {
    u8 w0x2;
    u8 w0x1;
    u8 w1x2;
    u8 w1x1;
} UnkWindow_020600e0;

typedef struct {
    void (*state)(void *);
    u8 pad4[4];
    u16 unk8;
} UnkTask_020600e0;

extern UnkWindow_020600e0 data_0208923c;

void func_0205fcf0(UnkTask_020600e0 *t);
void func_02060090(void *t);

#pragma thumb on
void func_020600e0(UnkTask_020600e0 *t) {
    UnkWindow_020600e0 *w;

    func_0205fcf0(t);
    w = (void *)&data_0208923c;
    w->w0x1 = 1;
    w->w0x2 = 0;
    w->w1x1 = 0;
    w->w1x2 = 1;
    t->unk8 = 0;
    t->state = (void (*)(void *))func_02060090;
}
#pragma thumb reset
