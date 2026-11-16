#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
} UnkAnim_022db810;

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[8];
    s32 unk10;
    u8 pad14[0xb44 - 0x14];
    UnkAnim_022db810 anims[14];
} UnkWork_022db810;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022db810 *work;
} UnkTask_022db810;

extern UnkTask_022db810 *gTaskCurrent;

void func_ov001_022da614(UnkWork_022db810 *w);
BOOL MsgBox_IsClosed(void);
void func_ov001_022da41c(UnkWork_022db810 *w);
BOOL MsgBox_GetResult(void);
void func_02050ea0(s32 a);
void func_ov001_022da374(UnkWork_022db810 *w);
void func_ov001_022da33c(UnkWork_022db810 *w);
void func_ov001_022d9f6c(UnkWork_022db810 *w);
void func_ov001_022d9f2c(UnkWork_022db810 *w);
void KeyframeAnim_SetAnim(UnkAnim_022db810 *a, s32 id);
void func_ov001_022d6dc0(void);
void func_ov001_022db458(void);
void func_ov001_022db9d8(void);

#pragma thumb on
void func_ov001_022db810(void) {
    UnkWork_022db810 *w;
    s32 i;

    w = gTaskCurrent->work;
    func_ov001_022da614(w);
    w->timer++;
    if (MsgBox_IsClosed()) {
        w->timer = 0;
        w->unk4 = 0;
        func_ov001_022da41c(w);
        if (!MsgBox_GetResult()) {
            func_02050ea0(w->unk10);
            func_ov001_022da374(w);
            func_ov001_022da33c(w);
            func_ov001_022d9f6c(w);
            func_ov001_022d9f2c(w);
            KeyframeAnim_SetAnim(&w->anims[0], 0xe);
            KeyframeAnim_SetAnim(&w->anims[1], 0xf);
            for (i = 0; i < 8; i++) {
                KeyframeAnim_SetAnim(&w->anims[6 + i], i + 0x14);
            }
            func_ov001_022d6dc0();
            gTaskCurrent->func = func_ov001_022db458;
            return;
        }
        gTaskCurrent->func = func_ov001_022db9d8;
    }
}
#pragma thumb reset
