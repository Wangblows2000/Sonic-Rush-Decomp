#include <nitro/types.h>

void Model_GlbFlushWVP(void *a, void *b, void *c, void *d);

void Model_MatrixOp_GlbFlushWVP(void *a, void *b, void *c, void *d) {
    Model_GlbFlushWVP(a, b, c, d);
}
