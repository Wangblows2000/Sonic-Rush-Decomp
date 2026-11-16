/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Options_Create();
s32 OptionsMenu_Create();

void Options_StartFromZoneMapPauseChoice3(void) {
    s32 t0 = Options_Create();
    OptionsMenu_Create(t0, 1, 1);
    *(volatile u32 *)0x4000000 = ((*(volatile u32 *)0x4000000 & -7937) | 0x1c00);
    *(volatile u32 *)0x4001000 = ((*(volatile u32 *)0x4001000 & -7937) | 0x1900);
}

#pragma thumb reset
