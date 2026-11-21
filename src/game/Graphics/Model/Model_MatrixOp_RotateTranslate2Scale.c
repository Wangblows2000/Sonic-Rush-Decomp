#include <nitro/types.h>

void NNS_G3dGeBufferOP_N(u32 a, void *b, u32 c);

void Model_MatrixOp_RotateTranslate2Scale(u8 *s) {
    NNS_G3dGeBufferOP_N(0x19, s + 0x24, 0xC);
    NNS_G3dGeBufferOP_N(0x1C, s + 0x54, 3);
    NNS_G3dGeBufferOP_N(0x1B, s + 0x18, 3);
}
