#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x48 - 8];
    s32 unk48;
} UnkWork_022dcdbc;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dcdbc *work;
} UnkTask_022dcdbc;

extern UnkTask_022dcdbc *gTaskCurrent;

void func_ov001_022da70c(UnkWork_022dcdbc *w);
BOOL func_02050d30(void);
BOOL func_02050dec(void);
void func_ov001_022d96c4(void);
BOOL func_ov001_022da52c(void);
void func_ov001_022da304(UnkWork_022dcdbc *w);
void func_02051f20(void);
BOOL func_ov001_022da4d4(void);
void func_ov001_022da450(UnkWork_022dcdbc *w);
void func_02050078(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void func_ov001_022dcd24(void);
void func_ov001_022da064(UnkWork_022dcdbc *w, s32 a);
void func_ov001_022dcc10(void);
void func_ov001_022da33c(UnkWork_022dcdbc *w);
void func_ov001_022da244(UnkWork_022dcdbc *w, s32 a);
void func_ov001_022da1cc(UnkWork_022dcdbc *w, s32 a);
void func_ov001_022d983c(UnkWork_022dcdbc *w, s32 a);
void func_ov001_022dccc8(void);

#pragma thumb on
void func_ov001_022dcdbc(void) {
    UnkWork_022dcdbc *w;
    BOOL a;
    BOOL b;
    BOOL c;

    w = gTaskCurrent->work;
    func_ov001_022da70c(w);
    a = FALSE;
    b = FALSE;
    c = FALSE;
    if (func_02050d30()) {
        b = TRUE;
    } else if (!func_02050dec()) {
        c = TRUE;
        func_ov001_022d96c4();
    } else if (func_ov001_022da52c()) {
        func_ov001_022da304(w);
        a = TRUE;
        func_02051f20();
    } else if (func_ov001_022da4d4()) {
        a = TRUE;
        func_02051f20();
    }
    w->timer++;
    if (a) {
        w->timer = 0;
        w->unk4 = 0;
        func_ov001_022da450(w);
        func_02050078(8, 3, 1, 1, 2, 0, 0xe, 0x200010);
        gTaskCurrent->func = func_ov001_022dcd24;
        return;
    }
    if (b) {
        w->timer = 0;
        w->unk4 = 0;
        func_ov001_022da064(w, 1);
        w->unk48 = 3;
        gTaskCurrent->func = func_ov001_022dcc10;
        return;
    }
    if (c) {
        w->timer = 0;
        w->unk4 = 0;
        func_ov001_022da33c(w);
        func_ov001_022da244(w, 1);
        func_ov001_022da1cc(w, 1);
        func_ov001_022d983c(w, 1);
        gTaskCurrent->func = func_ov001_022dccc8;
    }
}
#pragma thumb reset
