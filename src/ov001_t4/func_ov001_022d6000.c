#include <nitro/types.h>

typedef struct {
    s32 timer;
    u8 pad4[4];
    s32 state;
    s32 val;
    s32 timer2;
} UnkWork_022d6000;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d6000 *work;
} UnkTask_022d6000;

extern UnkTask_022d6000 *gTaskCurrent;

void func_ov001_022d5be4(UnkWork_022d6000 *w);
void func_ov001_022d5b44(UnkWork_022d6000 *w, s32 a);
s32 func_ov001_022d5550(s32 from, s32 to, s32 n, s32 i, s32 ease);
void func_ov001_022d5b94(UnkWork_022d6000 *w, s32 v);
void func_ov001_022d585c(UnkWork_022d6000 *w);
void func_ov001_022d59e0(UnkWork_022d6000 *w, s32 a, s32 v);
void func_ov001_022d5664(UnkWork_022d6000 *w, s32 v);
void func_ov001_022d5634(UnkWork_022d6000 *w, s32 v);
void func_ov001_022d586c(UnkWork_022d6000 *w);
BOOL func_ov001_022d5808(void);
void GXx_SetMasterBrightness_(vu16 *reg, s32 v);
void Task_DestroyAll(void);
void TitleDemo_SkipToTitleScreen(void);
void func_ov001_022d6534(s32 a);

#pragma thumb on
void func_ov001_022d6000(void) {
    UnkWork_022d6000 *w = gTaskCurrent->work;
    s32 t;
    s32 v;

    func_ov001_022d5be4(w);
    func_ov001_022d5b44(w, 0);
    t = w->timer - 0x38;
    if (t >= 0) {
        func_ov001_022d5b94(w, func_ov001_022d5550(0x100, 0, 0x10, t, 0x2000));
        if (t == 0) {
            func_ov001_022d585c(w);
        }
    }
    t = w->timer;
    if (t < 0x38) {
        v = func_ov001_022d5550(0, -0x20, 0x38, t, 0x1800);
    } else {
        v = func_ov001_022d5550(-0x20, -0x40, 0x10, t - 0x38, 0x800);
    }
    func_ov001_022d59e0(w, 0, v);
    t = w->timer;
    if (t < 0x38) {
        if (t >= 0) {
            v = func_ov001_022d5550(0x10, 0, 0x18, t, 0x1000);
            func_ov001_022d5664(w, 0);
            func_ov001_022d5634(w, v);
            if (t == 0) {
                func_ov001_022d586c(w);
            }
        } else {
            func_ov001_022d5664(w, 0);
            func_ov001_022d5634(w, 0x10);
        }
    } else {
        t -= 0x38;
        if (t >= 0) {
            v = func_ov001_022d5550(0, 0x10, 0x10, t, 0x1000);
            func_ov001_022d5664(w, 0);
            func_ov001_022d5634(w, v);
        } else {
            func_ov001_022d5664(w, 0);
            func_ov001_022d5634(w, 0);
        }
    }
    if (w->state == 0 && func_ov001_022d5808()) {
        w->state = 1;
        w->val = 0;
    }
    if (w->state == 1) {
        if (w->timer2 >= 0) {
            w->val = func_ov001_022d5550(0, 0x10, 0x20, w->timer2, 0x1000);
        }
        w->timer2++;
        if (w->timer2 >= 0x20) {
            func_ov001_022d5664(w, 0x10);
            func_ov001_022d5634(w, 0x10);
            GXx_SetMasterBrightness_((vu16 *)0x400006c, 0x10);
            GXx_SetMasterBrightness_((vu16 *)0x400106c, 0x10);
            Task_DestroyAll();
            TitleDemo_SkipToTitleScreen();
            return;
        }
    }
    w->timer++;
    if (w->timer >= 0x48) {
        func_ov001_022d5664(w, 0);
        func_ov001_022d5634(w, 0x10);
        GXx_SetMasterBrightness_((vu16 *)0x400006c, 0x10);
        func_ov001_022d6534(0);
    }
}
#pragma thumb reset
