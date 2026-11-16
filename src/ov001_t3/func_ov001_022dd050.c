#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x1c - 8];
    s32 unk1c;
    u8 pad20[0x48 - 0x20];
    s32 unk48;
    u8 unk4c[4];
} UnkWork_022dd050;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dd050 *work;
} UnkTask_022dd050;

extern UnkTask_022dd050 *gTaskCurrent;

void func_ov001_022da70c(UnkWork_022dd050 *w);
BOOL func_02050d30(void);
BOOL func_02050dec(void);
void func_ov001_022d96c4(void);
BOOL func_ov001_022da52c(void);
void func_ov001_022da304(UnkWork_022dd050 *w);
void func_02051f20(void);
BOOL func_ov001_022da4d4(void);
void func_ov001_022da450(UnkWork_022dd050 *w);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void func_ov001_022dcfec(void);
void func_ov001_022dcec8(void);
void func_ov001_022dcf94(void);
s32 func_02050d60(s32 a, u8 *buf, s32 n);
void func_ov001_022d9a54(void *p, u8 *buf, u16 len);
void func_ov001_022d9cd4(UnkWork_022dd050 *w, s32 a, s32 b);
BOOL func_ov001_022da500(void);
void func_ov001_022d9fe4(UnkWork_022dd050 *w);
void func_02051f90(void);
BOOL func_ov001_022da4bc(void);
void func_ov001_022da064(UnkWork_022dd050 *w, s32 a);
void func_ov001_022dcc10(void);
void func_ov001_022da33c(UnkWork_022dd050 *w);
void func_ov001_022da244(UnkWork_022dd050 *w, s32 a);
void func_ov001_022da1cc(UnkWork_022dd050 *w, s32 a);
void func_ov001_022d983c(UnkWork_022dd050 *w, s32 a);
void func_ov001_022dccc8(void);

#pragma thumb on
void func_ov001_022dd050(void) {
    UnkWork_022dd050 *w;
    BOOL a;
    BOOL b;
    BOOL c;
    u8 buf[20];

    w = gTaskCurrent->work;
    func_ov001_022d9a54(w->unk4c, buf, func_02050d60(0, buf, 7));
    func_ov001_022da70c(w);
    func_ov001_022d9cd4(w, 0, 0);
    a = FALSE;
    b = FALSE;
    c = FALSE;
    if (!func_02050dec()) {
        c = TRUE;
    } else if (func_ov001_022da52c()) {
        func_ov001_022da304(w);
        a = TRUE;
        func_02051f20();
    } else if (func_ov001_022da4d4()) {
        a = TRUE;
        func_02051f20();
    }
    if (func_ov001_022da500()) {
        func_ov001_022d9fe4(w);
        b = TRUE;
        w->unk4 = 0;
        func_02051f90();
    } else if (func_ov001_022da4bc()) {
        b = TRUE;
        func_02051f90();
    }
    w->timer++;
    if (a) {
        w->timer = 0;
        w->unk4 = 0;
        func_ov001_022da450(w);
        MsgBox_Create(8, 3, 1, 1, 2, 0, 0xe, 0x200010);
        gTaskCurrent->func = func_ov001_022dcfec;
        return;
    }
    if (b) {
        w->timer = 0;
        w->unk4 = 0;
        w->unk1c = 0;
        func_ov001_022da450(w);
        MsgBox_Create(7, 3, 1, 1, 2, 0, 0xe, 0x200010);
        gTaskCurrent->func = func_ov001_022dcec8;
        return;
    }
    if (c) {
        w->timer = 0;
        w->unk4 = 0;
        gTaskCurrent->func = func_ov001_022dcf94;
    }
}
#pragma thumb reset
