#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
} UnkAnim_022db8d0;

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0xb44 - 8];
    UnkAnim_022db8d0 anims[14];
} UnkWork_022db8d0;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_022db8d0 *work;
} UnkTask_022db8d0;

extern UnkTask_022db8d0 *gTaskCurrent;

void VsMenu_Unk_ov001_022da614(UnkWork_022db8d0 *w);
BOOL MsgBox_IsClosed(void);
void VsMenu_Unk_ov001_022da374(UnkWork_022db8d0 *w);
void VsMenu_Unk_ov001_022da33c(UnkWork_022db8d0 *w);
void VsMenu_Unk_ov001_022d9f6c(UnkWork_022db8d0 *w);
void VsMenu_Unk_ov001_022d9f2c(UnkWork_022db8d0 *w);
void KeyframeAnim_SetAnim(UnkAnim_022db8d0 *a, s32 idx);
void KeyframeAnim_PlayReverse(UnkAnim_022db8d0 *a);
void TitleMenu_Unk_ov001_022d6dac(void);
void VsMenu_State_Unk_ov001_022db6ec(void);

#pragma thumb on
void VsMenu_State_Unk_ov001_022db8d0(void) {
    UnkWork_022db8d0 *w = gTaskCurrent->work;
    s32 i;

    VsMenu_Unk_ov001_022da614(w);
    w->timer++;
    if (!MsgBox_IsClosed()) {
        return;
    }
    w->timer = 0;
    w->unk4 = 0;
    VsMenu_Unk_ov001_022da374(w);
    VsMenu_Unk_ov001_022da33c(w);
    VsMenu_Unk_ov001_022d9f6c(w);
    VsMenu_Unk_ov001_022d9f2c(w);
    KeyframeAnim_SetAnim(&w->anims[0], 0);
    KeyframeAnim_SetAnim(&w->anims[1], 1);
    KeyframeAnim_PlayReverse(&w->anims[0]);
    KeyframeAnim_PlayReverse(&w->anims[1]);
    for (i = 0; i < 8; i++) {
        UnkAnim_022db8d0 *a = &w->anims[i + 6];
        KeyframeAnim_SetAnim(a, i + 6);
        KeyframeAnim_PlayReverse(a);
    }
    TitleMenu_Unk_ov001_022d6dac();
    gTaskCurrent->fn = VsMenu_State_Unk_ov001_022db6ec;
}
#pragma thumb reset
