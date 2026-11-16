#include <nitro/types.h>

typedef struct {
    s32 mode;
    u32 timer;
    s32 frame;
    s32 bright;
} UnkWork_022e901c;

typedef struct {
    u8 pad0[0xa];
    s16 bright;
} UnkBright_022e901c;

extern UnkBright_022e901c gDisplayMain;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022e901c *work;
} UnkTask_022e901c;

extern UnkTask_022e901c *gTaskCurrent;

void func_ov001_022e8b5c(UnkWork_022e901c *w);
void func_ov001_022e8be0(UnkWork_022e901c *w, s32 i);
void func_ov001_022e9234(void *t);
void func_ov001_022e915c(void *t);

#pragma thumb on
void ZoneMapPause_State_Open(void) {
    UnkWork_022e901c *w = gTaskCurrent->work;
    s32 b = 0x48;

    b -= (s32)w->timer * 6;
    if (b < 0) {
        b = 0;
    }
    w->bright = b;
    gDisplayMain.bright = b;
    func_ov001_022e8b5c(w);
    switch (w->mode) {
    case 1:
        func_ov001_022e8be0(w, 0);
        func_ov001_022e8be0(w, 1);
        break;
    case 5:
        func_ov001_022e8be0(w, 2);
        func_ov001_022e8be0(w, 3);
        break;
    }
    w->timer++;
    w->frame++;
    if (b <= 0) {
        switch (w->mode) {
        case 1:
            func_ov001_022e9234(NULL);
            break;
        case 5:
            func_ov001_022e915c(NULL);
            break;
        }
    }
}
#pragma thumb reset
