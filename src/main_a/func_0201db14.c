#include <nitro/types.h>

void NNS_SndHeapClear(void *p);
void NNS_FndDestroyFrmHeap(void *p);

void func_0201db14(void **p) {
    NNS_SndHeapClear(p);
    NNS_FndDestroyFrmHeap(*p);
}
