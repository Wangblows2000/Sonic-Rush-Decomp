#include <nitro/types.h>

extern u8 gPad[];
void Pad_Reset(void *p, s32 a, s32 b);
void func_ov001_022cf4a0(s32 a);

#pragma thumb on
void TitleScreen_GoToUnlockNotice(void) {
    Pad_Reset(gPad, 0, 0);
    func_ov001_022cf4a0(1);
}
#pragma thumb reset
