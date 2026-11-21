/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Touch_Disable();
s32 TimeSelectMenu_ReleaseArchives();
s32 TimeSelectMenu_ReleaseNameGlyphs();
s32 TimeSelectMenu_ReleaseNopA();
s32 TimeSelectMenu_ReleaseNopB();
s32 TimeSelectMenu_ReleaseSprites();
s32 TimeSelectMenu_FreeBuffers();
s32 TimeSelectMenu_ReleaseDisplay();

void TimeSelectMenu_Destructor(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    Touch_Disable();
    TimeSelectMenu_ReleaseArchives(t0);
    TimeSelectMenu_ReleaseNameGlyphs(t0);
    TimeSelectMenu_ReleaseNopA(t0);
    TimeSelectMenu_ReleaseNopB(t0);
    TimeSelectMenu_ReleaseSprites(t0);
    TimeSelectMenu_FreeBuffers(t0);
    TimeSelectMenu_ReleaseDisplay();
}

#pragma thumb reset
