#include <nitro/types.h>

void NNS_G3dGlbFlushVP(void *a, void *b, void *c, void *d);

void Model_MatrixOp_FlushVP(void *a, void *b, void *c, void *d) {
    NNS_G3dGlbFlushVP(a, b, c, d);
}
