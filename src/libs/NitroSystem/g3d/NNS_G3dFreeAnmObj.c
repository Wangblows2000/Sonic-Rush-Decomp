#include <nitro/types.h>

void NNS_FndFreeToAllocator(void *a, void *b, void *c, void *d);

void NNS_G3dFreeAnmObj(void *a, void *b, void *c, void *d) {
    NNS_FndFreeToAllocator(a, b, c, d);
}
