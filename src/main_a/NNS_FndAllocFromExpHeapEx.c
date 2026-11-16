#include <nitro/types.h>

void *AllocFromHead_expheap(void *heap, u32 size, s32 align);
void *AllocFromTail_expheap(void *heap, u32 size, s32 align);

void *NNS_FndAllocFromExpHeapEx(void *heap, u32 size, s32 align) {
    if (size == 0) size = 1;
    size = (size + 3) & ~3;
    if (align >= 0) {
        return AllocFromHead_expheap(heap, size, align);
    } else {
        return AllocFromTail_expheap(heap, size, -align);
    }
}
