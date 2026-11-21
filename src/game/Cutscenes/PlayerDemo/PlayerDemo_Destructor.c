/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 PlayerDemo_FreeData();
s32 PlayerDemo_RestoreDisplay();
s32 BrightnessFade_Destroy();
s32 PlayerDemo_ReleaseSkippable();
s32 PlayerDemo_ReleaseSound();
extern u8 gPlayerDemoTask[];

void PlayerDemo_Destructor(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    PlayerDemo_FreeData(*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x10) + 0x14));
    PlayerDemo_RestoreDisplay(t0);
    BrightnessFade_Destroy();
    PlayerDemo_ReleaseSkippable(t0);
    PlayerDemo_ReleaseSound(t0);
    *(u32 *)gPlayerDemoTask = 0;
}

#pragma thumb reset
