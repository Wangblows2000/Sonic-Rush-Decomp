#include <nitro/types.h>

void *FindListContainHeap(void *p);
void NNS_FndRemoveListObject(void *a, void *b);

void NNSi_FndFinalizeHeap(void *p) {
    NNS_FndRemoveListObject(FindListContainHeap(p), p);
}
