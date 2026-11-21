/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 PlayerSelectMenu_IsFullyUnlocked();
extern u8 gSaveData[];

s32 PlayerSelectMenu_ShouldShowUnlockNotice(void) {
    s32 t0 = PlayerSelectMenu_IsFullyUnlocked();
    if (t0 == 0) {
        return 0;
    }
    if ((*(u32 *)((u8 *)gSaveData + 0x8) & 0x200000) == 0) {
        return 1;
    }
    return 0;
}

#pragma thumb reset
