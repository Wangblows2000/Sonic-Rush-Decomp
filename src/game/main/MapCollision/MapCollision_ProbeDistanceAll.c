#include <nitro/types.h>

s32 MapCollision_ProbeDistance(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
s32 ObjCollisionMap_ProbeDistance(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

s32 MapCollision_ProbeDistanceAll(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
    s32 r = MapCollision_ProbeDistance(a, b, c, d, e, f);
    s32 r2 = ObjCollisionMap_ProbeDistance(a, b, c, d, e, f);

    if (r > r2)
        r = r2;
    return r;
}
