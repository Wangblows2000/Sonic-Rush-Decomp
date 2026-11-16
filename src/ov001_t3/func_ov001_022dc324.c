#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
} UnkAnim_022dc324;

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x48 - 8];
    s32 unk48;
    u8 pad4c[0xb44 - 0x4c];
    UnkAnim_022dc324 anims[6];
} UnkWork_022dc324;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dc324 *work;
} UnkTask_022dc324;

extern UnkTask_022dc324 *data_022b4574;

void func_ov001_022da694(UnkWork_022dc324 *w);
BOOL func_020510c0(void);
BOOL func_020510f0(void);
void func_02040ec0(UnkAnim_022dc324 *a, s32 id);
void func_ov001_022d6dc0(void);
void func_ov001_022d97a8(s32 a, s32 b, s32 c, s32 d);
void func_02051db0(s32 a, s32 b, s32 c, s32 d);
void func_ov001_022dc0b4(void);
void func_ov001_022da33c(UnkWork_022dc324 *w);
void func_ov001_022da244(UnkWork_022dc324 *w, s32 a);
void func_ov001_022da1cc(UnkWork_022dc324 *w, s32 a);
void func_ov001_022d983c(UnkWork_022dc324 *w, s32 a);
void func_ov001_022d96c4(void);
void func_ov001_022dc734(void);

#pragma thumb on
void func_ov001_022dc324(void) {
    UnkWork_022dc324 *w;

    w = data_022b4574->work;
    func_ov001_022da694(w);
    w->timer++;
    if (func_020510c0()) {
        w->timer = 0;
        w->unk4 = 0;
        w->unk48 = 0;
        func_02040ec0(&w->anims[0], 0xe);
        func_02040ec0(&w->anims[1], 0xf);
        func_02040ec0(&w->anims[2], 0x10);
        func_02040ec0(&w->anims[3], 0x11);
        func_02040ec0(&w->anims[4], 0x12);
        func_02040ec0(&w->anims[5], 0x13);
        func_ov001_022d6dc0();
        func_ov001_022d97a8(0, 0x10, 0x10, 0xc);
        func_02051db0(2, 0xc, 0x3000, 0);
        data_022b4574->func = func_ov001_022dc0b4;
        return;
    }
    if (func_020510f0()) {
        func_ov001_022da33c(w);
        func_ov001_022da244(w, 1);
        func_ov001_022da1cc(w, 1);
        w->unk48 = 1;
        func_ov001_022d983c(w, 1);
        func_ov001_022d96c4();
        data_022b4574->func = func_ov001_022dc734;
    }
}
#pragma thumb reset
