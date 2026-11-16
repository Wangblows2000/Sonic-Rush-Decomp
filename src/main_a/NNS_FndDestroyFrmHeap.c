#include <nitro/types.h>

void NNSi_FndFinalizeHeap(void *a, void *b, void *c, void *d);

void NNS_FndDestroyFrmHeap(void *a, void *b, void *c, void *d) {
    NNSi_FndFinalizeHeap(a, b, c, d);
}
