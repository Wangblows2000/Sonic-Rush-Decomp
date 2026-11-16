#include <nitro/types.h>

typedef struct {
    u8 pad0[0x6c0];
    s32 unk6c0;
    u8 pad6c4[0xc];
    s32 unk6d0;
    u8 pad6d4[0x720 - 0x6d4];
    s32 unk720;
    u8 pad724[0xc];
    s32 unk730;
} UnkChild_022e61f4;

typedef struct {
    u8 pad0[8];
    s32 state;
    u8 padc[8];
    s32 unk14;
    u8 pad18[0x90 - 0x18];
    s32 unk90;
    u8 pad94[4];
    s32 unk98;
    u8 pad9c[8];
    UnkChild_022e61f4 *child;
} UnkWork_022e61f4;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022e61f4 *work;
} UnkTask_022e61f4;

extern UnkTask_022e61f4 *gTaskCurrent;

void func_ov001_022e5a48(s32 v);
void func_ov001_022e5e98(void);
void func_ov001_022e5e28(s32 a, s32 b, s32 c);
void func_ov001_022e5db0(s32 a);
void func_ov001_022e5de0(s32 a, s32 b);
void func_020538d8(void);
void func_ov001_022e7f64(void);

#pragma thumb on
void func_ov001_022e61f4(void) {
    UnkWork_022e61f4 *w = gTaskCurrent->work;

    switch (w->state) {
    case 0:
        w->unk98 = 1;
        func_ov001_022e5a48(0x3c);
        break;
    case 1:
        w->unk98 = 2;
        func_ov001_022e5e98();
        break;
    case 2:
        w->unk98 = 3;
        func_ov001_022e5e28(0xb, 0, 1);
        break;
    case 3:
        w->unk90 = 1;
        gTaskCurrent->func = func_ov001_022e7f64;
        break;
    case 4:
        w->unk98 = 5;
        func_ov001_022e5e98();
        break;
    case 5:
        w->unk98 = 6;
        func_ov001_022e5e28(0xc, 1, 0x10);
        break;
    case 6:
        w->unk98 = 7;
        func_ov001_022e5db0(0x10);
        break;
    case 7:
        w->unk98 = 8;
        func_ov001_022e5e98();
        break;
    case 8:
        w->unk98 = 10;
        func_020538d8();
        func_ov001_022e5a48(0xf0);
        break;
    case 10:
        w->unk98 = 0xb;
        func_ov001_022e5e28(0xd, 0x10, 2);
        break;
    case 11:
        w->unk90 = 1;
        gTaskCurrent->func = func_ov001_022e7f64;
        break;
    case 12:
        w->unk98 = 0xd;
        func_ov001_022e5e28(0xe, 2, 3);
        break;
    case 13:
        w->unk90 = 1;
        gTaskCurrent->func = func_ov001_022e7f64;
        break;
    case 14:
        w->unk98 = 0xf;
        func_ov001_022e5e28(0xf, 3, 4);
        break;
    case 15:
        w->unk90 = 1;
        gTaskCurrent->func = func_ov001_022e7f64;
        break;
    case 16:
        w->unk98 = 0x11;
        w->child->unk6c0 = 5;
        w->child->unk6d0 = 1;
        w->unk14 = 5;
        func_ov001_022e5e28(0x10, 4, 6);
        break;
    case 17:
        w->unk90 = 1;
        gTaskCurrent->func = func_ov001_022e7f64;
        break;
    case 18:
        w->unk98 = 0x13;
        func_ov001_022e5de0(5, 4);
        break;
    case 19:
        w->unk90 = 1;
        gTaskCurrent->func = func_ov001_022e7f64;
        break;
    case 20:
        w->unk98 = 0x15;
        w->child->unk6d0 = 1;
        func_ov001_022e5e28(0x11, 5, 8);
        break;
    case 21:
        w->unk90 = 1;
        gTaskCurrent->func = func_ov001_022e7f64;
        break;
    case 22:
        w->unk98 = 0x17;
        func_ov001_022e5e28(0x12, 8, 10);
        break;
    case 23:
        w->unk90 = 1;
        gTaskCurrent->func = func_ov001_022e7f64;
        break;
    case 24:
        w->unk98 = 0x19;
        w->child->unk720 = 5;
        w->child->unk730 = 1;
        w->unk14 = 0xb;
        func_ov001_022e5e28(0x13, 10, 0xc);
        break;
    case 25:
        w->unk90 = 1;
        gTaskCurrent->func = func_ov001_022e7f64;
        break;
    case 26:
        w->unk98 = 0x1b;
        func_ov001_022e5de0(0xb, 10);
        break;
    case 27:
        w->unk90 = 1;
        gTaskCurrent->func = func_ov001_022e7f64;
        break;
    case 28:
        w->unk98 = 0x1d;
        w->child->unk730 = 1;
        func_ov001_022e5e28(0x14, 0xb, 0xd);
        break;
    case 29:
        w->unk90 = 1;
        gTaskCurrent->func = func_ov001_022e7f64;
        break;
    case 30:
        w->unk98 = 0x1f;
        func_ov001_022e5e98();
        break;
    case 31:
        w->unk98 = 0x20;
        func_ov001_022e5e28(0x15, 0xd, 0xe);
        break;
    case 32:
        w->unk90 = 1;
        gTaskCurrent->func = func_ov001_022e7f64;
        break;
    case 33:
        w->unk90 = 1;
        gTaskCurrent->func = func_ov001_022e7f64;
        break;
    }
}
#pragma thumb reset
