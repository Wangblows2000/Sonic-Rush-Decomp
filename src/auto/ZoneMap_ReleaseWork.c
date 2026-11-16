/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 ZoneMap_ReleaseNodeSprites();
s32 ZoneMap_Unk_ov001_022e5620();
s32 ZoneMap_ReleaseCharModel();
s32 ZoneMap_ReleaseScrollArrows();

void ZoneMap_ReleaseWork(s32 p0) {
    ZoneMap_ReleaseNodeSprites(((u8 *)p0 + 0x674));
    ZoneMap_Unk_ov001_022e5620(((u8 *)p0 + 0x4));
    ZoneMap_ReleaseCharModel(((u8 *)p0 + 0x1994));
    ZoneMap_ReleaseScrollArrows(((u8 *)p0 + 0x1b08));
}

#pragma thumb reset
