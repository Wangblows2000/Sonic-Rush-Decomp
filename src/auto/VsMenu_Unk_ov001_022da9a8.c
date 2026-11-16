/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 VsMenu_Unk_ov001_022da374();
s32 VsMenu_Unk_ov001_022da33c();
s32 KeyframeAnim_SetAnim();
s32 KeyframeAnim_PlayReverse();
s32 TitleMenu_Unk_ov001_022d6dac();
void VsMenu_State_Unk_ov001_022dbdb0();
extern u8 gTaskCurrent[];

void VsMenu_Unk_ov001_022da9a8(s32 p0) {
    VsMenu_Unk_ov001_022da374();
    VsMenu_Unk_ov001_022da33c(p0);
    KeyframeAnim_SetAnim(((u8 *)p0 + 0xb44), 0x1c);
    KeyframeAnim_SetAnim(((u8 *)p0 + 0xb70), 0x1d);
    KeyframeAnim_PlayReverse(((u8 *)p0 + 0xb44));
    KeyframeAnim_PlayReverse(((u8 *)p0 + 0xb70));
    TitleMenu_Unk_ov001_022d6dac();
    *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)VsMenu_State_Unk_ov001_022dbdb0;
}

#pragma thumb reset
