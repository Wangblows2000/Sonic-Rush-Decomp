#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x48 - 8];
    s32 unk48;
} UnkWork_022dc410;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_022dc410 *work;
} UnkTask_022dc410;

extern UnkTask_022dc410 *gTaskCurrent;

void func_ov001_022da694(UnkWork_022dc410 *w);
BOOL func_02051130(void);
void func_0205004c(void);
BOOL func_02050018(void);
void func_ov001_022da41c(UnkWork_022dc410 *w);
BOOL func_0205000c(void);
void func_02051110(void);
void func_ov001_022da33c(UnkWork_022dc410 *w);
void func_ov001_022da244(UnkWork_022dc410 *w, s32 a);
void func_ov001_022da1cc(UnkWork_022dc410 *w, s32 a);
void func_ov001_022d983c(UnkWork_022dc410 *w, s32 i);
void func_ov001_022dc324(void);
void func_ov001_022dc734(void);

#pragma thumb on
void func_ov001_022dc410(void) {
    UnkWork_022dc410 *w = gTaskCurrent->work;

    func_ov001_022da694(w);
    if (func_02051130()) {
        func_0205004c();
    }
    w->timer++;
    if (func_02050018()) {
        w->timer = 0;
        w->unk4 = 0;
        func_ov001_022da41c(w);
        if (!func_0205000c()) {
            func_02051110();
            gTaskCurrent->fn = func_ov001_022dc324;
            return;
        }
        func_ov001_022da33c(w);
        func_ov001_022da244(w, 1);
        func_ov001_022da1cc(w, 1);
        w->unk48 = 1;
        func_ov001_022d983c(w, 1);
        gTaskCurrent->fn = func_ov001_022dc734;
    }
}
#pragma thumb reset
