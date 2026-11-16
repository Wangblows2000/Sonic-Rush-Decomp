#include <nitro/types.h>

void RingManager_SpawnRing(s32 a, s32 b, s32 c, s32 d, s32 e);

s32 Ring_Create(s32 unused, s32 a, s32 b) {
    RingManager_SpawnRing(a, b, 0, 0, 0x10);
    return 0;
}
