#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
} UnkAnim_022dbe3c;

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x14 - 8];
    s32 unk14;
    u8 pad18[4];
    s32 unk1c;
    u8 pad20[0xb44 - 0x20];
    UnkAnim_022dbe3c anims[2];
} UnkWork_022dbe3c;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dbe3c *work;
} UnkTask_022dbe3c;

extern UnkTask_022dbe3c *gTaskCurrent;

void func_ov001_022d9e50(UnkWork_022dbe3c *w, s32 x, s32 y);
void func_ov001_022d9e2c(UnkWork_022dbe3c *w, s32 x, s32 y);
void func_ov001_022d9ab4(UnkWork_022dbe3c *w, s32 a, s32 b, s32 c, s32 d);
BOOL func_ov001_022da52c(void);
void func_ov001_022da304(UnkWork_022dbe3c *w);
void MsgBox_RequestClose(void);
void func_02051f20(void);
BOOL func_ov001_022da4d4(void);
s32 func_02050f60(void);
void func_ov001_022dab54(UnkWork_022dbe3c *w);
BOOL MsgBox_IsClosed(void);
void func_ov001_022da41c(UnkWork_022dbe3c *w);
void func_02051070(void);
void func_ov001_022da9a8(UnkWork_022dbe3c *w);
void func_ov001_022da374(UnkWork_022dbe3c *w);
void func_ov001_022da33c(UnkWork_022dbe3c *w);
void KeyframeAnim_SetAnim(UnkAnim_022dbe3c *a, s32 id);
void func_ov001_022d6dc0(void);
void func_ov001_022d96b8(void);
void func_ov001_022dbcd8(void);

#pragma thumb on
void func_ov001_022dbe3c(void) {
    UnkWork_022dbe3c *w;

    w = gTaskCurrent->work;
    func_ov001_022d9e50(w, 0x38, 0x98);
    func_ov001_022d9e2c(w, 0x10, 0x48);
    func_ov001_022d9ab4(w, 0xb0, 8, 0, 0);
    if (func_ov001_022da52c()) {
        func_ov001_022da304(w);
        MsgBox_RequestClose();
        w->unk1c = 1;
        func_02051f20();
    } else if (func_ov001_022da4d4()) {
        MsgBox_RequestClose();
        w->unk1c = 1;
        func_02051f20();
    } else if (func_02050f60() >= 1) {
        MsgBox_RequestClose();
        w->unk14 = func_02050f60();
        w->unk1c = 0;
    }
    func_ov001_022dab54(w);
    w->timer++;
    if (MsgBox_IsClosed()) {
        w->timer = 0;
        w->unk4 = 0;
        func_ov001_022da41c(w);
        if (w->unk1c != 0) {
            func_02051070();
            func_ov001_022da9a8(w);
            return;
        }
        func_ov001_022da374(w);
        func_ov001_022da33c(w);
        KeyframeAnim_SetAnim(&w->anims[0], 0x1e);
        KeyframeAnim_SetAnim(&w->anims[1], 0x1f);
        func_ov001_022d6dc0();
        func_ov001_022d96b8();
        gTaskCurrent->func = func_ov001_022dbcd8;
    }
}
#pragma thumb reset
