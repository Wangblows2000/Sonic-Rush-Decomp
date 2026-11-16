#include <nitro/types.h>

typedef struct {
    s32 step;
    s32 unk4;
    s32 mode;
} UnkWork_022dd42c;

typedef struct {
    u8 pad[0x10];
    UnkWork_022dd42c *work;
} UnkTask_022dd42c;

extern UnkTask_022dd42c *data_022b4574;

void func_ov001_022ddc3c(UnkWork_022dd42c *w);
void func_ov001_022ddb00(UnkWork_022dd42c *w);
void func_ov001_022dd93c(UnkWork_022dd42c *w);
void func_ov001_022ddacc(UnkWork_022dd42c *w);
void func_ov001_022dd614(UnkTask_022dd42c *t);
void func_ov001_022dd574(UnkTask_022dd42c *t);
void func_ov001_022dd534(UnkTask_022dd42c *t);

#pragma thumb on
void func_ov001_022dd42c(void) {
    UnkWork_022dd42c *w = data_022b4574->work;

    if (w->step == 0) {
        func_ov001_022ddc3c(w);
    } else if (w->step == 1) {
        func_ov001_022ddb00(w);
    } else if (w->step == 2) {
        func_ov001_022dd93c(w);
    } else if (w->step == 3) {
        func_ov001_022ddacc(w);
    }
    w->step++;
    if (w->step >= 4) {
        w->step = 0;
        switch (w->mode) {
        case 0:
            func_ov001_022dd614(data_022b4574);
            break;
        case 1:
            func_ov001_022dd574(data_022b4574);
            break;
        case 2:
            func_ov001_022dd534(data_022b4574);
            break;
        }
    }
}
#pragma thumb reset
