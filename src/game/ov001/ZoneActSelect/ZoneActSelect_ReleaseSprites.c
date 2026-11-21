#include <nitro/types.h>

typedef struct { u8 data[0x48]; } UnkStruct_022e14b8;
void ZoneActSelect_ReleaseSprite(void *p);

#pragma thumb on
void ZoneActSelect_ReleaseSprites(u8 *p) {
    s32 i;
    for (i = 0; i < 0x26; i++) {
        ZoneActSelect_ReleaseSprite(&((UnkStruct_022e14b8 *)(p + 0x360))[i]);
    }
}
#pragma thumb reset
