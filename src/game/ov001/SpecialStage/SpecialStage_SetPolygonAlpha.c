#include <nitro/types.h>

void NNS_G3dGeBufferOP_N(s32 a, void *b, s32 c);

void SpecialStage_SetPolygonAlpha(s32 v) {
    s32 a = (v << 16) | 0xc0;

    NNS_G3dGeBufferOP_N(0x29, &a, 1);
}
