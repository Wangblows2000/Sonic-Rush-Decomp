#include <nitro/types.h>

typedef struct {
    s32 timer;
    u8 pad4[4];
    s32 state;
    s32 val;
    s32 timer2;
} UnkWork_022d5d70;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d5d70 *work;
} UnkTask_022d5d70;

extern UnkTask_022d5d70 *gTaskCurrent;

void func_ov001_022d5be4(UnkWork_022d5d70 *w);
s32 func_ov001_022d5550(s32 from, s32 to, s32 n, s32 i, s32 ease);
void func_ov001_022d5664(UnkWork_022d5d70 *w, s32 v);
void func_ov001_022d5634(UnkWork_022d5d70 *w, s32 v);
BOOL func_ov001_022d5808(void);
void GXx_SetMasterBrightness_(vu16 *reg, s32 v);
void Task_DestroyAll(void);
void func_ov001_022d5834(void);
void func_ov001_022d6470(s32 a);
void func_ov001_022d5b44(UnkWork_022d5d70 *w, s32 a);
void func_ov001_022d5b94(UnkWork_022d5d70 *w, s32 a);
void func_ov001_022d5a18(UnkWork_022d5d70 *w);
void func_ov001_022d583c(UnkWork_022d5d70 *w);
void func_ov001_022d57b4(UnkWork_022d5d70 *w);
void func_ov001_022d5738(UnkWork_022d5d70 *w);
void func_ov001_022d592c(UnkWork_022d5d70 *w, s32 a, s32 b);
void func_ov001_022d58ac(UnkWork_022d5d70 *w, s32 a, s32 b);

#pragma thumb on
void func_ov001_022d5d70(void) {
    UnkWork_022d5d70 *w;

    w = gTaskCurrent->work;
    func_ov001_022d5be4(w);
    func_ov001_022d5b44(w, 0);
    func_ov001_022d5b94(w, 0);
    if (w->timer >= 0) {
        func_ov001_022d5a18(w);
    }
    if (w->timer == 0) {
        func_ov001_022d583c(w);
    }
    func_ov001_022d57b4(w);
    func_ov001_022d5738(w);
    func_ov001_022d592c(w, 0x28, -0x40);
    func_ov001_022d58ac(w, -0x28, -0x40);
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
            func_ov001_022d5834();
            return;
        }
    }
    func_ov001_022d5664(w, 0);
    func_ov001_022d5634(w, 0);
    w->timer++;
    if (w->timer >= 10) {
        func_ov001_022d6470(0);
    }
}
#pragma thumb reset
