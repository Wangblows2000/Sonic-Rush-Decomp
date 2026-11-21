/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 VsMenu_Unk_ov001_022da374();
s32 VsMenu_Unk_ov001_022da33c();
s32 KeyframeAnim_SetAnim();
void VsMenu_State_ChildOpen();

void VsMenu_Action_ChildOpen(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    VsMenu_Unk_ov001_022da374(*(u32 *)((u8 *)p0 + 0x10));
    VsMenu_Unk_ov001_022da33c(t0);
    KeyframeAnim_SetAnim((t0 + 0xb44), 0x1c);
    KeyframeAnim_SetAnim((t0 + 0xb70), 0x1d);
    *(u32 *)((u8 *)p0 + 0x8) = (u32)VsMenu_State_ChildOpen;
}

#pragma thumb reset
