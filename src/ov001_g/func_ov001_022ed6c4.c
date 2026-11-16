#include <nitro/types.h>

typedef struct UnkWork_022ed6c4 {
    s32 timer;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    u8 pad28[8];
    s32 unk30;
} UnkWork_022ed6c4;

typedef struct UnkTask_022ed6c4 {
    u8 pad0[8];
    void (*func)(void);
    u8 padC[4];
    UnkWork_022ed6c4 *work;
} UnkTask_022ed6c4;

typedef struct UnkTouch_022ed6c4 {
    u8 pad0[8];
    u16 x;
    u16 y;
} UnkTouch_022ed6c4;

extern UnkTask_022ed6c4 *gTaskCurrent;
extern UnkTouch_022ed6c4 gTouchState;

void func_ov001_022ed490(UnkWork_022ed6c4 *w, s16 x, s16 y);
void func_ov001_022ed3e0(UnkWork_022ed6c4 *w);
void func_ov001_022ed390(UnkWork_022ed6c4 *w, s32 flag);
void func_ov001_022ed19c(UnkWork_022ed6c4 *w);
void func_ov001_022ed188(UnkWork_022ed6c4 *w);
void func_ov001_022ed14c(UnkWork_022ed6c4 *w);
void func_ov001_022ed104(UnkWork_022ed6c4 *w);
void func_ov001_022ed08c(UnkWork_022ed6c4 *w);
BOOL func_ov001_022eced8(UnkWork_022ed6c4 *w);
BOOL func_ov001_022ecec0(UnkWork_022ed6c4 *w);
BOOL func_ov001_022ece98(UnkWork_022ed6c4 *w);
BOOL func_ov001_022ecf5c(UnkWork_022ed6c4 *w);
BOOL func_ov001_022ecf04(UnkWork_022ed6c4 *w);
s32 func_ov001_022ece04(UnkWork_022ed6c4 *w, s32 v);
s32 func_ov001_022ecdcc(UnkWork_022ed6c4 *w, s32 v);
void SysSound_PlaySE(s32 a);
void Records_CreateFadeTask(s32 a, s32 b, s32 c, s32 d);
void Records_State_ExitToOptions(void);
void func_ov001_022ed548(void);
void func_ov001_022ed614(void);

#pragma thumb on
void func_ov001_022ed6c4(void) {
    UnkWork_022ed6c4 *w = gTaskCurrent->work;
    BOOL f1, f2, f3;

    func_ov001_022ed490(w, 0, 0x90);
    func_ov001_022ed3e0(w);
    func_ov001_022ed390(w, 0);
    func_ov001_022ed19c(w);
    func_ov001_022ed188(w);
    func_ov001_022ed14c(w);
    func_ov001_022ed104(w);
    func_ov001_022ed08c(w);
    f1 = FALSE;
    f2 = FALSE;
    f3 = FALSE;
    if (func_ov001_022eced8(w)) {
        f1 = TRUE;
        w->unk24 = 1;
    } else if (func_ov001_022ecec0(w)) {
        w->unk24 = 0;
        f1 = TRUE;
    } else if (func_ov001_022ece98(w)) {
        f2 = TRUE;
        w->unk4 = 4;
        w->unk8 = 4;
        w->unk14 = w->unk30;
        w->unkC = gTouchState.x;
        w->unk10 = gTouchState.y;
        w->unk18 = 0;
    } else if (func_ov001_022ecf5c(w)) {
        f3 = TRUE;
        w->unk1C = w->unk30;
        w->unk20 = func_ov001_022ece04(w, w->unk30);
        w->unk4 = 0;
        w->unk8 = 0;
    } else if (func_ov001_022ecf04(w)) {
        f3 = TRUE;
        w->unk1C = w->unk30;
        w->unk20 = func_ov001_022ecdcc(w, w->unk30);
        w->unk4 = 1;
        w->unk8 = 1;
    } else {
        w->unk4 = 6;
        w->unk8 = 6;
    }
    w->timer++;
    if (f1) {
        SysSound_PlaySE(1);
        w->timer = 0;
        w->unk4 = 6;
        w->unk8 = 6;
        Records_CreateFadeTask(0, -16, 16, 0);
        gTaskCurrent->func = Records_State_ExitToOptions;
    } else if (f2) {
        w->timer = 0;
        gTaskCurrent->func = func_ov001_022ed548;
    } else if (f3) {
        w->timer = 0;
        gTaskCurrent->func = func_ov001_022ed614;
    }
}
#pragma thumb reset
