#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
} UnkSprite_022e36c4;

typedef struct {
    u8 pad0[0xc];
    UnkSprite_022e36c4 spr[6];
} UnkStruct_022e36c4;

void ZoneMap_ReleaseSprite(UnkSprite_022e36c4 *s);

#pragma thumb on
void ZoneMap_ReleaseScrollArrows(UnkStruct_022e36c4 *p) {
    s32 i;

    for (i = 0; i < 4; i++) {
        ZoneMap_ReleaseSprite(&p->spr[i]);
    }
    ZoneMap_ReleaseSprite(&p->spr[4]);
    ZoneMap_ReleaseSprite(&p->spr[5]);
}
#pragma thumb reset
