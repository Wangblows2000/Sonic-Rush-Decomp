/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022da374();
s32 func_ov001_022da33c();
s32 func_ov001_022da2cc();
s32 func_ov001_022da288();
s32 KeyframeAnim_SetAnim();
s32 KeyframeAnim_PlayReverse();
s32 func_ov001_022d6dac();
void func_ov001_022daea0();
extern u8 gTaskCurrent[];

void func_ov001_022da850(s32 p0) {
    func_ov001_022da374();
    func_ov001_022da33c(p0);
    func_ov001_022da2cc(p0, 0);
    func_ov001_022da2cc(p0, 1);
    func_ov001_022da288(p0, 0);
    func_ov001_022da288(p0, 1);
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
    func_ov001_022d6dac();
    *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov001_022daea0;
}

#pragma thumb reset
