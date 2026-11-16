#include <nitro/types.h>

void NNS_G3dGeBufferOP_N(u32 a, void *b, u32 c);

void Model_MatrixOp_Translate(u8 *s) {
    NNS_G3dGeBufferOP_N(0x1C, s + 0x48, 3);
}
