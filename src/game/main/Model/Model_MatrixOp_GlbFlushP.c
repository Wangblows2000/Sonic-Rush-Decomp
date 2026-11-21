#include <nitro/types.h>

void Model_GlbFlushP(void *a, void *b, void *c, void *d);

void Model_MatrixOp_GlbFlushP(void *a, void *b, void *c, void *d) {
    Model_GlbFlushP(a, b, c, d);
}
