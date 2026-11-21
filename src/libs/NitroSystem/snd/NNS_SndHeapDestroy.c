#include <nitro/types.h>

void NNS_SndHeapClear(void *p);
void NNS_FndDestroyFrmHeap(void *p);

void NNS_SndHeapDestroy(void **p) {
    NNS_SndHeapClear(p);
    NNS_FndDestroyFrmHeap(*p);
}
