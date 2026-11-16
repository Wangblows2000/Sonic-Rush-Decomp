#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x1c - 8];
    s32 unk1c;
    u8 pad20[0x48 - 0x20];
    s32 unk48;
    u8 unk4c[4];
} UnkWork_022dc580;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dc580 *work;
} UnkTask_022dc580;

extern UnkTask_022dc580 *data_022b4574;

void func_ov001_022da70c(UnkWork_022dc580 *w);
BOOL func_02050d30(void);
BOOL func_02050dec(void);
void func_ov001_022d96c4(void);
BOOL func_ov001_022da52c(void);
void func_ov001_022da304(UnkWork_022dc580 *w);
void func_02051f20(void);
BOOL func_ov001_022da4d4(void);
void func_ov001_022da450(UnkWork_022dc580 *w);
void func_02050078(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void func_ov001_022dc4f0(void);
void func_ov001_022dc410(void);
void func_ov001_022dc498(void);
void func_ov001_022da694(UnkWork_022dc580 *w);
s32 func_02051150(u8 *buf, s32 n);
BOOL func_02051130(void);
s32 func_02050d60(s32 a, u8 *buf, s32 n);
void func_ov001_022d9a54(void *p, u8 *buf, u16 len);
void func_ov001_022d9cd4(UnkWork_022dc580 *w, s32 a, s32 b);
BOOL func_ov001_022da500(void);
void func_ov001_022d9fe4(UnkWork_022dc580 *w);
void func_02051f90(void);
BOOL func_ov001_022da4bc(void);
void func_ov001_022da064(UnkWork_022dc580 *w, s32 a);
void func_ov001_022dcc10(void);
void func_ov001_022da33c(UnkWork_022dc580 *w);
void func_ov001_022da244(UnkWork_022dc580 *w, s32 a);
void func_ov001_022da1cc(UnkWork_022dc580 *w, s32 a);
void func_ov001_022d983c(UnkWork_022dc580 *w, s32 a);
void func_ov001_022dccc8(void);

#pragma thumb on
void func_ov001_022dc580(void) {
    UnkWork_022dc580 *w;
    BOOL a;
    BOOL c;
    u8 buf[16];

    w = data_022b4574->work;
    func_ov001_022da694(w);
    func_ov001_022d9cd4(w, 0, 0);
    func_ov001_022d9a54(w->unk4c, buf, func_02051150(buf, 7));
    a = FALSE;
    c = FALSE;
    if (w->unk1c == 0) {
        if (func_ov001_022da52c()) {
            func_ov001_022da304(w);
            a = TRUE;
            func_02051f20();
        } else if (func_ov001_022da4d4()) {
            a = TRUE;
            func_02051f20();
        }
        if (func_ov001_022da500()) {
            func_ov001_022d9fe4(w);
            w->unk1c = 1;
            w->unk4 = 0;
            func_02051f90();
        } else if (func_ov001_022da4bc()) {
            w->unk1c = 1;
            w->unk4 = 0x10;
            func_02051f90();
        } else if (func_02051130()) {
            c = TRUE;
        }
    }
    w->timer++;
    w->unk4++;
    if (a) {
        w->timer = 0;
        w->unk4 = 0;
        func_ov001_022da450(w);
        func_02050078(1, 3, 1, 1, 2, 0, 0xe, 0x200010);
        data_022b4574->func = func_ov001_022dc4f0;
        return;
    }
    if (w->unk1c != 0 && w->unk4 > 0x10) {
        w->timer = 0;
        w->unk4 = 0;
        w->unk1c = 0;
        func_ov001_022da450(w);
        func_02050078(6, 3, 1, 1, 2, 0, 0xe, 0x200010);
        data_022b4574->func = func_ov001_022dc410;
        return;
    }
    if (c) {
        w->timer = 0;
        w->unk4 = 0;
        data_022b4574->func = func_ov001_022dc498;
    }
}
#pragma thumb reset
