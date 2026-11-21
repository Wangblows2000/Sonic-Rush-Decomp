#include <nitro/types.h>

void *AllocFromHead_frameheap(void *hd, u32 size, s32 align);
void *AllocFromTail_frameheap(void *hd, u32 size, s32 align);

void *NNS_FndAllocFromFrmHeapEx(u8 *heap, u32 size, s32 align) {
    void *hd = heap + 0x24;
    if (size == 0) size = 1;
    size = (size + 3) & ~3;
    if (align >= 0) {
        return AllocFromHead_frameheap(hd, size, align);
    } else {
        return AllocFromTail_frameheap(hd, size, -align);
    }
}
