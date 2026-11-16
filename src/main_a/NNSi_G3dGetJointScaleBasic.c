#include <nitro/types.h>

typedef struct { u32 x, y, z; } UnkVec_020284ec;
typedef struct { u32 flags; UnkVec_020284ec v; } UnkStruct_020284ec;

void NNSi_G3dGetJointScaleBasic(UnkStruct_020284ec *s, const UnkVec_020284ec *v, u32 c, u32 flags) {
    if (flags & 4) {
        s->flags |= 1;
    } else {
        s->v.x = v->x;
        s->v.y = v->y;
        s->v.z = v->z;
    }
    s->flags |= 0x18;
}
