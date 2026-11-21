#include <nitro/types.h>

typedef struct { fx32 x, y, z; } UnkVec_020229a0;

void blendScaleVec_(UnkVec_020229a0 *dst, const UnkVec_020229a0 *src, fx32 s, BOOL uniform) {
    if (uniform) {
        dst->x += s;
        dst->y += s;
        dst->z += s;
    } else {
        dst->x += (s * src->x) >> 12;
        dst->y += (s * src->y) >> 12;
        dst->z += (s * src->z) >> 12;
    }
}
