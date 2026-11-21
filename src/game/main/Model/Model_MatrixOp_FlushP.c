#include <nitro/types.h>

void NNS_G3dGlbFlushP(void *a, void *b, void *c, void *d);

void Model_MatrixOp_FlushP(void *a, void *b, void *c, void *d) {
    NNS_G3dGlbFlushP(a, b, c, d);
}
