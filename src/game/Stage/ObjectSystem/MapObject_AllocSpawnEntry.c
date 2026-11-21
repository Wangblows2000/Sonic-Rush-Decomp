#include <nitro/types.h>

extern u32 gMapObjectSpawnMaskLo;
extern u32 gMapObjectSpawnMaskHi;

u8 MapObject_AllocSpawnEntry(void) {
    u32 a = gMapObjectSpawnMaskLo;
    u32 b = gMapObjectSpawnMaskHi;
    u8 i;
    for (i = 0; i < 0x28; i++) {
        if (i < 0x20) {
            if (!(a & (1 << i))) {
                gMapObjectSpawnMaskLo |= 1 << i;
                return i;
            }
        } else {
            if (!(b & (1 << (i - 0x20)))) {
                gMapObjectSpawnMaskHi |= 1 << (i - 0x20);
                return i;
            }
        }
    }
    return i;
}
