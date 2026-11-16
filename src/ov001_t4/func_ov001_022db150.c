#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
} UnkAnim_022db150;

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[0xb44 - 8];
    UnkAnim_022db150 anims[6];
} UnkWork_022db150;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022db150 *work;
} UnkTask_022db150;

extern UnkTask_022db150 *data_022b4574;

void func_ov001_022da59c(UnkWork_022db150 *w);
void func_ov001_022d9cb0(UnkWork_022db150 *w, s32 a, s32 b);
BOOL func_ov001_022da52c(void);
BOOL func_ov001_022da4d4(void);
BOOL func_02050e50(void);
BOOL func_02050e80(void);
void func_ov001_022d96c4(void);
void func_ov001_022da304(UnkWork_022db150 *w);
void func_02051f20(void);
void func_ov001_022da450(UnkWork_022db150 *w);
void func_02050078(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void func_ov001_022da374(UnkWork_022db150 *w);
void func_ov001_022da33c(UnkWork_022db150 *w);
void func_ov001_022da2cc(UnkWork_022db150 *w, s32 i);
void func_ov001_022da288(UnkWork_022db150 *w, s32 i);
void func_02040ec0(UnkAnim_022db150 *a, s32 id);
void func_ov001_022d6dc0(void);
void func_ov001_022d97a8(s32 a, s32 b, s32 c, s32 d);
void func_02051db0(s32 a, s32 b, s32 c, s32 d);
void func_ov001_022db014(void);
void func_ov001_022dac0c(void);
void func_ov001_022dafdc(void);

#pragma thumb on
void func_ov001_022db150(void) {
    UnkWork_022db150 *w = data_022b4574->work;
    BOOL cancel;
    BOOL ok;
    BOOL other;

    func_ov001_022da59c(w);
    func_ov001_022d9cb0(w, 0, 0);
    cancel = FALSE;
    ok = FALSE;
    other = FALSE;
    if (func_ov001_022da52c()) {
        func_ov001_022da304(w);
        cancel = TRUE;
        func_02051f20();
    } else if (func_ov001_022da4d4()) {
        cancel = TRUE;
        func_02051f20();
    } else if (func_02050e50()) {
        ok = TRUE;
    } else if (func_02050e80()) {
        other = TRUE;
        func_ov001_022d96c4();
    }
    w->unk0++;
    if (cancel) {
        w->unk0 = 0;
        w->unk4 = 0;
        func_ov001_022da450(w);
        func_02050078(3, 3, 1, 1, 2, 0, 0xe, 0x200010);
        data_022b4574->func = func_ov001_022db014;
        return;
    }
    if (ok) {
        w->unk0 = 0;
        w->unk4 = 0;
        func_ov001_022da374(w);
        func_ov001_022da33c(w);
        func_ov001_022da2cc(w, 0);
        func_ov001_022da2cc(w, 1);
        func_ov001_022da288(w, 0);
        func_ov001_022da288(w, 1);
        func_02040ec0(&w->anims[0], 0xe);
        func_02040ec0(&w->anims[1], 0xf);
        func_02040ec0(&w->anims[2], 0x10);
        func_02040ec0(&w->anims[3], 0x11);
        func_02040ec0(&w->anims[4], 0x12);
        func_02040ec0(&w->anims[5], 0x13);
        func_ov001_022d6dc0();
        func_ov001_022d97a8(0, 0x10, 0x10, 0xc);
        func_02051db0(2, 0xc, 0x3000, 0);
        data_022b4574->func = func_ov001_022dac0c;
        return;
    }
    if (other) {
        w->unk0 = 0;
        w->unk4 = 0;
        func_ov001_022da450(w);
        func_02050078(2, 3, 2, 1, 2, 0, 0xe, 0x200010);
        data_022b4574->func = func_ov001_022dafdc;
    }
}
#pragma thumb reset
