#include <nitro/types.h>

void FreeHead(void *p);
void FreeTail(void *p);

void NNS_FndFreeToFrmHeap(void *p, u32 flags) {
    if (flags & 1) FreeHead(p);
    if (!(flags & 2)) return;
    FreeTail(p);
}
