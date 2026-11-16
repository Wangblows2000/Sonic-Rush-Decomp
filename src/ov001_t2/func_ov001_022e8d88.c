#include <nitro/types.h>

typedef struct {
    s32 mode;
    u32 timer;
    s32 frame;
} UnkWork_022e8d88;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022e8d88 *work;
} UnkTask_022e8d88;

extern UnkTask_022e8d88 *data_022b4574;

void func_ov001_022e8b5c(UnkWork_022e8d88 *w);
void func_ov001_022e8be0(UnkWork_022e8d88 *w, s32 i);
void func_ov001_022e91cc(void *t);
void func_ov001_022e90f4(void *t);

#pragma thumb on
void func_ov001_022e8d88(void) {
    UnkWork_022e8d88 *w = data_022b4574->work;

    func_ov001_022e8b5c(w);
    switch (w->mode) {
    case 3:
        func_ov001_022e8be0(w, 0);
        func_ov001_022e8be0(w, 1);
        break;
    case 7:
        func_ov001_022e8be0(w, 2);
        func_ov001_022e8be0(w, 3);
        break;
    }
    w->timer++;
    w->frame++;
    if (w->timer >= 0x18) {
        switch (w->mode) {
        case 3:
            func_ov001_022e91cc(NULL);
            break;
        case 7:
            func_ov001_022e90f4(NULL);
            break;
        }
    }
}
#pragma thumb reset
