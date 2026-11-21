#include <nitro/types.h>

void NNS_G3dGeBufferOP_N(u32 a, void *b, u32 c);

void Model_MatrixOp_MultMtx43(u8 *s) {
    NNS_G3dGeBufferOP_N(0x19, s + 0x60, 0xC);
}
