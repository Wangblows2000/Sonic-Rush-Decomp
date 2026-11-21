/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 TimeSelectMenu_DrawSprite588();
s32 TimeSelectMenu_DrawSprite2B8();
s32 TimeSelectMenu_DrawSprite300();
s32 TimeSelectMenu_DrawSpriteGroupB();
s32 TimeSelectMenu_DrawSpriteGroupA();
s32 TimeSelectMenu_DrawPlayerName();
s32 TimeSelectMenu_DrawSprite5D0();
s32 TimeSelectMenu_DrawSprite618();
s32 TimeSelectMenu_DrawSpriteScaled();
s32 TimeSelectMenu_SetupOrthoCamera();
s32 TimeSelectMenu_DrawModelB();
s32 TimeSelectMenu_DrawModelA();
s32 TimeSelectMenu_DrawNop();

void TimeSelectMenu_DrawLayoutA(u8 *p0) {
    TimeSelectMenu_DrawSprite588(p0, 8, 0x88);
    TimeSelectMenu_DrawSprite588(p0, 0x98, 0x88);
    if (*(u32 *)((u8 *)p0 + 0x28) == 0) {
        TimeSelectMenu_DrawSprite2B8(p0, 8, 0x98);
        TimeSelectMenu_DrawSprite300(p0, 0x98, 0x98);
        TimeSelectMenu_DrawSpriteGroupB(p0, 0x38, 0x58, 0x1000);
        TimeSelectMenu_DrawSpriteGroupA(p0, 0xc8, 0x58, 0x1000);
    } else {
        TimeSelectMenu_DrawSprite300(p0, 8, 0x98);
        TimeSelectMenu_DrawSprite2B8(p0, 0x98, 0x98);
        TimeSelectMenu_DrawSpriteGroupA(p0, 0x38, 0x58, 0x1000);
        TimeSelectMenu_DrawSpriteGroupB(p0, 0xc8, 0x58, 0x1000);
    }
    TimeSelectMenu_DrawPlayerName(p0, 8, 0, 0);
    TimeSelectMenu_DrawSprite5D0(p0, 8, 0);
    TimeSelectMenu_DrawPlayerName(p0, 0x98, 0, 1);
    TimeSelectMenu_DrawSprite5D0(p0, 0x98, 0);
    TimeSelectMenu_DrawSprite618(p0, 0x80, 0x58);
    TimeSelectMenu_DrawSpriteScaled(p0, 0x80, 0x58, 0x1000);
    TimeSelectMenu_SetupOrthoCamera(p0);
    if (*(u32 *)((u8 *)p0 + 0x28) == 0) {
        TimeSelectMenu_DrawModelB(p0, 0x38, 0x90, 1);
        TimeSelectMenu_DrawModelA(p0, 0xc8, 0x90, 0);
    } else {
        TimeSelectMenu_DrawModelA(p0, 0x38, 0x90, 1);
        TimeSelectMenu_DrawModelB(p0, 0xc8, 0x90, 0);
    }
    TimeSelectMenu_DrawNop(p0);
}

#pragma thumb reset
