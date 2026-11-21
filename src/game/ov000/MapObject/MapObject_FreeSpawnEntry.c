#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
} UnkStruct_022fae34;

extern UnkStruct_022fae34 gMapObjectSpawnEntries[];
extern u32 gMapObjectSpawnMaskLo;
extern u32 gMapObjectSpawnMaskHi;

void MapObject_FreeSpawnEntry(UnkStruct_022fae34 *p) {
    UnkStruct_022fae34 *e;
    u8 i;

    e = gMapObjectSpawnEntries;
    for (i = 0; i < 40; i++, e++) {
        if (e == p) {
            if (i < 32)
                gMapObjectSpawnMaskLo &= ~(1 << i);
            else
                gMapObjectSpawnMaskHi &= ~(1 << (i - 32));
            return;
        }
    }
}
