#include <nitro/types.h>

typedef struct {
    fx32 x;
    fx32 y;
    fx32 z;
} UnkVec_020222bc;

typedef struct {
    u8 unk0[0xe0];
    UnkVec_020222bc scale;
    UnkVec_020222bc trans;
    u8 unkF8[4];
    u32 flag;
} UnkGlb_020222bc;

extern UnkGlb_020222bc NNS_G3dGlb;

void NNS_G3dGlbSetBaseScale(const UnkVec_020222bc *trans)
{
    NNS_G3dGlb.trans = *trans;
    NNS_G3dGlb.flag &= ~0xa4;
}
