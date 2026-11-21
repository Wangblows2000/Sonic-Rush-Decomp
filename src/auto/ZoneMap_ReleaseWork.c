/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 ZoneMap_ReleaseNodeSprites();
s32 ZoneMap_ReleaseMapBg();
s32 ZoneMap_ReleaseCharModel();
s32 ZoneMap_ReleaseScrollArrows();

void ZoneMap_ReleaseWork(s32 p0) {
    ZoneMap_ReleaseNodeSprites(((u8 *)p0 + 0x674));
    ZoneMap_ReleaseMapBg(((u8 *)p0 + 0x4));
    ZoneMap_ReleaseCharModel(((u8 *)p0 + 0x1994));
    ZoneMap_ReleaseScrollArrows(((u8 *)p0 + 0x1b08));
}

#pragma thumb reset
