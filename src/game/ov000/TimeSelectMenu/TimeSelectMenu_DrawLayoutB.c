/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 TimeSelectMenu_DrawSprite150();
s32 TimeSelectMenu_DrawSprite30();
s32 TimeSelectMenu_DrawSprite78();
s32 TimeSelectMenu_DrawSpriteC0();
s32 TimeSelectMenu_DrawSprite0Scaled();
s32 TimeSelectMenu_DrawSprite198();

void TimeSelectMenu_DrawLayoutB(u8 *p0, s32 p1, s32 p2) {
    TimeSelectMenu_DrawSprite150(p0, 0, 0);
    if (*(u32 *)((u8 *)p0 + 0x4) != 0) {
        TimeSelectMenu_DrawSprite30(p0, 0, 0);
    }
    TimeSelectMenu_DrawSprite78(p0, 0x60, 0x90);
    TimeSelectMenu_DrawSpriteC0(p0, 0xfa, 0x90);
    TimeSelectMenu_DrawSprite0Scaled(p0, 0x48, 0xa0, p1);
    if (*(u32 *)((u8 *)p0 + 0x4) != 0) {
        if ((s32)p2 != 0) {
            TimeSelectMenu_DrawSprite198(p0, 8, 0x90);
        }
    }
}

#pragma thumb reset
