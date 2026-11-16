/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Touch_Disable();
s32 TimeSelectMenu_ReleaseArchives();
s32 TimeSelectMenu_ReleaseNameGlyphs();
s32 func_ov000_023622f0();
s32 func_ov000_023622ec();
s32 TimeSelectMenu_ReleaseSprites();
s32 TimeSelectMenu_FreeBuffers();
s32 TimeSelectMenu_ReleaseDisplay();

void TimeSelectMenu_Destructor(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    Touch_Disable();
    TimeSelectMenu_ReleaseArchives(t0);
    TimeSelectMenu_ReleaseNameGlyphs(t0);
    func_ov000_023622f0(t0);
    func_ov000_023622ec(t0);
    TimeSelectMenu_ReleaseSprites(t0);
    TimeSelectMenu_FreeBuffers(t0);
    TimeSelectMenu_ReleaseDisplay();
}

#pragma thumb reset
