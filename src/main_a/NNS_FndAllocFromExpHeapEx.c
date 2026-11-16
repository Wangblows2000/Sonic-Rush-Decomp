#include <nitro/types.h>

void *AllocFromHead(void *heap, u32 size, s32 align);
void *AllocFromTail(void *heap, u32 size, s32 align);

void *NNS_FndAllocFromExpHeapEx(void *heap, u32 size, s32 align) {
    if (size == 0) size = 1;
    size = (size + 3) & ~3;
    if (align >= 0) {
        return AllocFromHead(heap, size, align);
    } else {
        return AllocFromTail(heap, size, -align);
    }
}
