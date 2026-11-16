/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 VsMenu_Unk_ov001_022da3ac();
s32 VsMenu_Unk_ov001_022da33c();
s32 VsMenu_Unk_ov001_022da2cc();
s32 VsMenu_Unk_ov001_022da288();
s32 VsMenu_Unk_ov001_022da244();
s32 KeyframeAnim_SetAnim();
void VsMenu_Unk_ov001_022dc81c();

void VsMenu_Unk_ov001_022dd574(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    VsMenu_Unk_ov001_022da3ac(*(u32 *)((u8 *)p0 + 0x10));
    VsMenu_Unk_ov001_022da33c(t0);
    VsMenu_Unk_ov001_022da2cc(t0, 0);
    VsMenu_Unk_ov001_022da2cc(t0, 1);
    VsMenu_Unk_ov001_022da288(t0, 0);
    VsMenu_Unk_ov001_022da244(t0, 1);
    KeyframeAnim_SetAnim((t0 + 0xb44), 0);
    KeyframeAnim_SetAnim((t0 + 0xb70), 1);
    KeyframeAnim_SetAnim((t0 + 0xb9c), 2);
    KeyframeAnim_SetAnim((t0 + 0xbc8), 3);
    KeyframeAnim_SetAnim((t0 + 0xbf4), 4);
    KeyframeAnim_SetAnim((t0 + 0xc20), 5);
    *(u32 *)((u8 *)t0 + 0x20) = 0;
    *(u32 *)((u8 *)t0 + 0x48) = 1;
    *(u32 *)((u8 *)p0 + 0x8) = (u32)VsMenu_Unk_ov001_022dc81c;
}

#pragma thumb reset
