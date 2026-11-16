#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 timer2;
    u8 pad8[0x1c - 8];
    s32 unk1c;
    u8 pad20[0x48 - 0x20];
    s32 unk48;
} UnkWork_022dcec8;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dcec8 *work;
} UnkTask_022dcec8;

extern UnkTask_022dcec8 *gTaskCurrent;

void func_ov001_022da70c(UnkWork_022dcec8 *w);
void func_ov001_022da024(UnkWork_022dcec8 *w);
void func_ov001_022d9cd4(UnkWork_022dcec8 *w, s32 a, s32 b);
BOOL func_02050dec(void);
void func_0205004c(void);
BOOL func_02050018(void);
void func_ov001_022da41c(UnkWork_022dcec8 *w);
void func_ov001_022da33c(UnkWork_022dcec8 *w);
void func_ov001_022da244(UnkWork_022dcec8 *w, s32 a);
void func_ov001_022da1cc(UnkWork_022dcec8 *w, s32 a);
void func_ov001_022d983c(UnkWork_022dcec8 *w, s32 a);
BOOL func_0205000c(void);
void func_02050d50(void);
void func_ov001_022da0dc(UnkWork_022dcec8 *w, s32 a);
void func_ov001_022daaa8(UnkWork_022dcec8 *w);
void func_ov001_022dd1ec(void);
void func_ov001_022dcdbc(void);

#pragma thumb on
void func_ov001_022dcec8(void) {
    UnkWork_022dcec8 *w;

    w = gTaskCurrent->work;
    func_ov001_022da70c(w);
    if (w->timer2 >= 0x10) {
        func_ov001_022da024(w);
    }
    func_ov001_022d9cd4(w, 0, 0);
    if (!func_02050dec()) {
        func_0205004c();
        w->unk1c = 1;
    }
    w->timer++;
    w->timer2++;
    if (func_02050018()) {
        w->timer = 0;
        w->timer2 = 0;
        func_ov001_022da41c(w);
        if (w->unk1c == 1) {
            w->unk1c = 0;
            func_ov001_022da33c(w);
            func_ov001_022da244(w, 1);
            func_ov001_022da1cc(w, 1);
            w->unk48 = 1;
            func_ov001_022d983c(w, 1);
            gTaskCurrent->func = func_ov001_022dd1ec;
            return;
        }
        if (!func_0205000c()) {
            func_02050d50();
            func_ov001_022da0dc(w, 1);
            w->unk48 = 2;
            gTaskCurrent->func = func_ov001_022dcdbc;
            return;
        }
        func_ov001_022daaa8(w);
    }
}
#pragma thumb reset
