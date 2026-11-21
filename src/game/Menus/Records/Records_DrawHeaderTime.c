/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Records_DrawTwoDigits();

void Records_DrawHeaderTime(s32 p0) {
    Records_DrawTwoDigits(p0, 0x68, 0x26, *(u8 *)((u8 *)p0 + 0x38), 1);
    Records_DrawTwoDigits(p0, 0x90, 0x26, *(u8 *)((u8 *)p0 + 0x39), 1);
    Records_DrawTwoDigits(p0, 0xb8, 0x26, *(u8 *)((u8 *)p0 + 0x3a), 1);
}

#pragma thumb reset
