#include <nitro/types.h>

typedef struct {
    s32 mode;
    s32 timer;
    s32 timer2;
    s32 pos;
    s32 unk10;
} UnkWork_022e8f6c;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022e8f6c *work;
} UnkTask_022e8f6c;

typedef struct {
    u8 pad0[8];
    u16 unk8;
    s16 unka;
} UnkStruct_0208922c;

extern UnkTask_022e8f6c *gTaskCurrent;
extern UnkStruct_0208922c gDisplayMain;

void func_ov001_022e8b5c(UnkWork_022e8f6c *w);
void func_ov001_022e8be0(UnkWork_022e8f6c *w, s32 n);
void func_ov001_022e9194(s32 a);
void func_ov001_022e90b8(s32 a);

#pragma thumb on
void func_ov001_022e8f6c(void) {
    UnkWork_022e8f6c *w;
    s32 v;

    w = gTaskCurrent->work;
    v = 0;
    v += w->timer * 6;
    if (v > 0x48) v = 0x48;
    w->pos = v;
    gDisplayMain.unka = v;
    func_ov001_022e8b5c(w);
    switch (w->mode) {
    case 4:
        func_ov001_022e8be0(w, 0);
        func_ov001_022e8be0(w, 1);
        break;
    case 8:
        func_ov001_022e8be0(w, 2);
        func_ov001_022e8be0(w, 3);
        break;
    }
    w->timer++;
    w->timer2++;
    if (v >= 0x48) {
        switch (w->mode) {
        case 4:
            if (w->unk10 == 0) {
                func_ov001_022e9194(0);
            } else {
                func_ov001_022e90b8(0);
            }
            break;
        case 8:
            func_ov001_022e90b8(0);
            break;
        }
    }
}
#pragma thumb reset
