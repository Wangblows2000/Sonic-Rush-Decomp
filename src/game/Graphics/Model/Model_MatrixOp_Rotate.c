#include <nitro/types.h>

void NNS_G3dGeBufferOP_N(u32 a, void *b, u32 c);

void Model_MatrixOp_Rotate(u8 *s) {
    NNS_G3dGeBufferOP_N(0x1A, s + 0x24, 9);
}
