#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
} UnkAnim_022da91c;

typedef struct {
    u8 pad0[0xb44];
    UnkAnim_022da91c anims[14];
} UnkWork_022da91c;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
} UnkTask_022da91c;

extern UnkTask_022da91c *gTaskCurrent;

void VsMenu_Unk_ov001_022da374(UnkWork_022da91c *w);
void VsMenu_Unk_ov001_022da33c(UnkWork_022da91c *w);
void VsMenu_Unk_ov001_022d9f6c(UnkWork_022da91c *w);
void VsMenu_Unk_ov001_022d9f2c(UnkWork_022da91c *w);
void KeyframeAnim_SetAnim(UnkAnim_022da91c *a, s32 idx);
void KeyframeAnim_PlayReverse(UnkAnim_022da91c *a);
void TitleMenu_StartVsBgSlideReverse(void);
void VsMenu_State_Unk_ov001_022db5f4(void);

#pragma thumb on
void VsMenu_Action_ChildCancel(UnkWork_022da91c *w) {
    s32 i;

    VsMenu_Unk_ov001_022da374(w);
    VsMenu_Unk_ov001_022da33c(w);
    VsMenu_Unk_ov001_022d9f6c(w);
    VsMenu_Unk_ov001_022d9f2c(w);
    KeyframeAnim_SetAnim(&w->anims[0], 0);
    KeyframeAnim_SetAnim(&w->anims[1], 1);
    KeyframeAnim_PlayReverse(&w->anims[0]);
    KeyframeAnim_PlayReverse(&w->anims[1]);
    for (i = 0; i < 8; i++) {
        UnkAnim_022da91c *a = &w->anims[i + 6];
        KeyframeAnim_SetAnim(a, i + 6);
        KeyframeAnim_PlayReverse(a);
    }
    TitleMenu_StartVsBgSlideReverse();
    gTaskCurrent->fn = VsMenu_State_Unk_ov001_022db5f4;
}
#pragma thumb reset
