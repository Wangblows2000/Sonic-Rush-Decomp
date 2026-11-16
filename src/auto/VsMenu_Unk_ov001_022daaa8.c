/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 KeyframeAnim_SetAnim();
s32 KeyframeAnim_PlayReverse();
s32 VsMenu_ClearNameSlot2();
s32 TitleMenu_Unk_ov001_022d6dac();
void VsMenu_State_Unk_ov001_022dcad4();
extern u8 gTaskCurrent[];

void VsMenu_Unk_ov001_022daaa8(s32 p0) {
    KeyframeAnim_SetAnim(((u8 *)p0 + 0xb44), 0);
    KeyframeAnim_SetAnim(((u8 *)p0 + 0xb70), 1);
    KeyframeAnim_SetAnim(((u8 *)p0 + 0xb9c), 2);
    KeyframeAnim_SetAnim(((u8 *)p0 + 0xbc8), 3);
    KeyframeAnim_SetAnim(((u8 *)p0 + 0xbf4), 4);
    KeyframeAnim_SetAnim(((u8 *)p0 + 0xc20), 5);
    KeyframeAnim_PlayReverse(((u8 *)p0 + 0xb44));
    KeyframeAnim_PlayReverse(((u8 *)p0 + 0xb70));
    KeyframeAnim_PlayReverse(((u8 *)p0 + 0xb9c));
    KeyframeAnim_PlayReverse(((u8 *)p0 + 0xbc8));
    KeyframeAnim_PlayReverse(((u8 *)p0 + 0xbf4));
    KeyframeAnim_PlayReverse(((u8 *)p0 + 0xc20));
    VsMenu_ClearNameSlot2(p0, 1);
    TitleMenu_Unk_ov001_022d6dac();
    *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)VsMenu_State_Unk_ov001_022dcad4;
}

#pragma thumb reset
