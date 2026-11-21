#include <nitro/types.h>

BOOL NNS_FndAllocFromExpHeapEx(void *a, void *b, void *c);
void OS_GetArenaLo(u32 a);

void Heap_Alloc(void *a, void *b, void *c) {
    if (NNS_FndAllocFromExpHeapEx(a, b, c)) return;
    OS_GetArenaLo(0);
}
