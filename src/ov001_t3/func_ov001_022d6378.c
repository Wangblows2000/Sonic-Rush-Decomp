#include <nitro/types.h>

typedef struct {
    s32 timer;
    u8 pad4[4];
    s32 state;
    s32 val;
    s32 timer2;
} UnkWork_022d6378;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d6378 *work;
} UnkTask_022d6378;

extern UnkTask_022d6378 *data_022b4574;

void func_ov001_022d5be4(UnkWork_022d6378 *w);
s32 func_ov001_022d5550(s32 from, s32 to, s32 n, s32 i, s32 ease);
void func_ov001_022d5664(UnkWork_022d6378 *w, s32 v);
void func_ov001_022d5634(UnkWork_022d6378 *w, s32 v);
BOOL func_ov001_022d5808(void);
void GXx_SetMasterBrightness_(vu16 *reg, s32 v);
void func_02037134(void);
void func_ov001_022d5834(void);
void func_ov001_022d65dc(s32 a);

#pragma thumb on
void func_ov001_022d6378(void) {
    UnkWork_022d6378 *w;

    w = data_022b4574->work;
    func_ov001_022d5be4(w);
    if (w->timer >= 0) {
        func_ov001_022d5664(w, func_ov001_022d5550(0x10, 0, 0x20, w->timer, 0x1000));
        func_ov001_022d5634(w, 0x10);
    } else {
        func_ov001_022d5664(w, 0x10);
        func_ov001_022d5634(w, 0x10);
    }
    if (w->state == 0 && w->timer > 0 && func_ov001_022d5808()) {
        w->state = 1;
        w->val = 0;
        w->timer2 = 0;
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
            func_02037134();
            func_ov001_022d5834();
            return;
        }
    }
    w->timer++;
    if (w->timer >= 0x20) {
        func_ov001_022d5664(w, 0);
        func_ov001_022d5634(w, 0x10);
        func_ov001_022d65dc(0);
    }
}
#pragma thumb reset
