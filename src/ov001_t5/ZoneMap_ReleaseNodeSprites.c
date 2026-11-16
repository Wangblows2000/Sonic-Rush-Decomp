#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
} UnkSprite_022e4e64;

typedef struct {
    u8 pad0[0x9ac];
    UnkSprite_022e4e64 a[21];
    u8 padf94[0x1004 - 0xf94];
    UnkSprite_022e4e64 c[3];
    UnkSprite_022e4e64 b[8];
} UnkStruct_022e4e64;

void ZoneMap_ReleaseSprite(UnkSprite_022e4e64 *s);

#pragma thumb on
void ZoneMap_ReleaseNodeSprites(UnkStruct_022e4e64 *p) {
    s32 i;

    for (i = 0; i < 21; i++) {
        ZoneMap_ReleaseSprite(&p->a[i]);
    }
    for (i = 0; i < 8; i++) {
        ZoneMap_ReleaseSprite(&p->b[i]);
    }
    for (i = 0; i < 3; i++) {
        ZoneMap_ReleaseSprite(&p->c[i]);
    }
}
#pragma thumb reset
