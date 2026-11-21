/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Touch_Disable();
s32 ResultsScreen_ReleaseNameGlyphs();
s32 ResultsScreen_ReleaseArchives();
s32 ResultsScreen_ReleaseNop();
s32 ResultsScreen_ReleaseSprites();
s32 ResultsScreen_FreeBuffers();
s32 ResultsScreen_ResetBgOffsets();

void ResultsScreen_Destructor(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    Touch_Disable();
    ResultsScreen_ReleaseNameGlyphs(t0);
    ResultsScreen_ReleaseArchives(t0);
    ResultsScreen_ReleaseNop(t0);
    ResultsScreen_ReleaseSprites(t0);
    ResultsScreen_FreeBuffers(t0);
    ResultsScreen_ResetBgOffsets();
}

#pragma thumb reset
