#include <nitro/types.h>

typedef struct UnkStruct_02310054_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_02310054_v;

typedef struct UnkStruct_02310054 {
    u8 pad0[0x20];
    UnkStruct_02310054_v pos;
} UnkStruct_02310054;

extern u8 data_02084d84[];

void MTX_MultVec43(UnkStruct_02310054_v *v, void *m, UnkStruct_02310054_v *out);
BOOL NNS_G3dWorldPosToScrPos(UnkStruct_02310054_v *v, s32 *x, s32 *y);

BOOL func_ov001_02310054(UnkStruct_02310054 *p) {
    s32 sx;
    s32 sy;
    UnkStruct_02310054_v out;

    if (p->pos.z < -0x258000)
        return TRUE;
    if (p->pos.x > 0x64000 || p->pos.x < -0x64000)
        return TRUE;
    MTX_MultVec43(&p->pos, data_02084d84, &out);
    if (out.z != 0 && !NNS_G3dWorldPosToScrPos(&p->pos, &sx, &sy))
        return FALSE;
    if (p->pos.z > 0x19000)
        return TRUE;
    if (p->pos.y > 0xc8000 || p->pos.y < -0xc8000)
        return TRUE;
    return FALSE;
}
