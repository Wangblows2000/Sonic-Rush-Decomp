/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 TimeAttackMenu_ReleaseSprites();
s32 TimeAttackMenu_Dummy();
s32 TimeAttackMenu_ReleaseAssets();
s32 TimeAttackMenu_ResetDisplayThunk();
s32 Touch_Disable();
extern u8 gPad[];
s32 Pad_Reset();

void TimeAttackMenu_Destructor(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    TimeAttackMenu_ReleaseSprites(*(u32 *)((u8 *)p0 + 0x10));
    TimeAttackMenu_Dummy(t0);
    TimeAttackMenu_ReleaseAssets(t0);
    TimeAttackMenu_ResetDisplayThunk();
    Touch_Disable();
    Pad_Reset(gPad, 0, 0);
}

#pragma thumb reset
