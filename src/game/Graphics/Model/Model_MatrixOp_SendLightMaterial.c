#include <nitro/types.h>

void Model_GlbSendLightMaterial(void *a, void *b, void *c, void *d);

void Model_MatrixOp_SendLightMaterial(void *a, void *b, void *c, void *d) {
    Model_GlbSendLightMaterial(a, b, c, d);
}
