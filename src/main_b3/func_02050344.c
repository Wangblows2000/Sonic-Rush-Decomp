#include <nitro/types.h>

typedef struct { u8 pad0[8]; s32 unk8; s32 unkC; } UnkWork_02050344;
typedef struct { u8 pad0[0x10]; UnkWork_02050344 *work; } UnkTask_02050344;
extern UnkTask_02050344 *gTaskCurrent;
extern s32 data_022c463c;
extern void *data_022c4638;

void func_02051838(void);
s32 func_02042958(void);
s32 func_02050c60(void);
void func_020429d0(void);
void func_020516b4(void);

#pragma thumb on
void func_02050344(void) {
    UnkWork_02050344 *w = gTaskCurrent->work;

    if (w->unkC != 0) {
        func_02051838();
        data_022c463c = 0;
        return;
    }
    switch (func_02042958()) {
    case 2:
        if (w->unk8 != 0) {
            w->unk8 = 0;
            if (func_02050c60() > 0) {
                func_020429d0();
                return;
            }
            func_02051838();
            data_022c463c = 1;
            return;
        }
        break;
    case 6:
        func_020516b4();
        func_02051838();
        data_022c463c = 0;
        data_022c4638 = NULL;
        return;
    case 7:
        func_02051838();
        data_022c463c = 1;
        break;
    case 0:
    case 1:
    case 3:
    case 4:
    case 5:
        break;
    }
}
#pragma thumb reset
