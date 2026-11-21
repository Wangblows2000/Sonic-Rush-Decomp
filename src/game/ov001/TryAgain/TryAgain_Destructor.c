/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Touch_Disable();
extern u8 gSystemFlags[];
s32 TryAgain_Dummy();
s32 TryAgain_ReleaseSprites();
s32 TryAgain_ReleaseAssets();
s32 TryAgain_ResetDisplay();

void TryAgain_Destructor(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    Touch_Disable();
    s32 t1 = (*(u32 *)gSystemFlags & -1025);
    *(u32 *)gSystemFlags = t1;
    TryAgain_Dummy(t0, gSystemFlags, t1);
    TryAgain_ReleaseSprites(t0);
    TryAgain_ReleaseAssets(t0);
    TryAgain_ResetDisplay();
}

#pragma thumb reset
