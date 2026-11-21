#include <nitro/types.h>

void Model_GlbFlushVP(void *a, void *b, void *c, void *d);

void Model_MatrixOp_GlbFlushVP(void *a, void *b, void *c, void *d) {
    Model_GlbFlushVP(a, b, c, d);
}
