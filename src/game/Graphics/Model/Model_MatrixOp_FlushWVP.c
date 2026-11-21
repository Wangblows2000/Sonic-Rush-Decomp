#include <nitro/types.h>

void NNS_G3dGlbFlushWVP(void *a, void *b, void *c, void *d);

void Model_MatrixOp_FlushWVP(void *a, void *b, void *c, void *d) {
    NNS_G3dGlbFlushWVP(a, b, c, d);
}
