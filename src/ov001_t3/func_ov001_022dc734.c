#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x1c - 8];
    s32 unk1c;
    u8 pad20[0x48 - 0x20];
    s32 unk48;
    u8 unk4c[4];
} UnkWork_022dc734;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dc734 *work;
} UnkTask_022dc734;

extern UnkTask_022dc734 *gTaskCurrent;

void func_ov001_022da694(UnkWork_022dc734 *w);
BOOL func_ov001_022da52c(void);
void func_ov001_022da304(UnkWork_022dc734 *w);
void SysSound_PlayCancelSE(void);
BOOL func_ov001_022da4d4(void);
BOOL func_020511a0(void);
void func_ov001_022da450(UnkWork_022dc734 *w);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void func_ov001_022dc6d8(void);
s32 func_02051150(u8 *buf, s32 n);
void func_ov001_022d9a54(void *p, u8 *buf, u16 len);
void func_ov001_022da024(UnkWork_022dc734 *w);
void func_ov001_022da288(UnkWork_022dc734 *w, s32 a);
void func_ov001_022da154(UnkWork_022dc734 *w, s32 a);
void func_ov001_022d96b8(void);
void func_ov001_022dc580(void);

#pragma thumb on
void func_ov001_022dc734(void) {
    UnkWork_022dc734 *w;
    BOOL a;
    BOOL b;
    u8 buf[16];

    w = gTaskCurrent->work;
    func_ov001_022da694(w);
    a = FALSE;
    b = FALSE;
    if (func_ov001_022da52c()) {
        func_ov001_022da304(w);
        a = TRUE;
        SysSound_PlayCancelSE();
    } else if (func_ov001_022da4d4()) {
        a = TRUE;
        SysSound_PlayCancelSE();
    } else if (func_020511a0()) {
        b = TRUE;
    }
    w->timer++;
    if (a) {
        w->timer = 0;
        w->unk4 = 0;
        func_ov001_022da450(w);
        MsgBox_Create(1, 3, 1, 1, 2, 0, 0xe, 0x200010);
        gTaskCurrent->func = func_ov001_022dc6d8;
        return;
    }
    if (b) {
        w->timer = 0;
        w->unk4 = 0;
        func_ov001_022d9a54(w->unk4c, buf, func_02051150(buf, 7));
        func_ov001_022da024(w);
        func_ov001_022da288(w, 1);
        func_ov001_022da154(w, 1);
        w->unk48 = 0;
        w->unk1c = 0;
        func_ov001_022d96b8();
        gTaskCurrent->func = func_ov001_022dc580;
    }
}
#pragma thumb reset
