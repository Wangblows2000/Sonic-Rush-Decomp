#include <nitro/types.h>

void *InitExpHeap(void *start, void *end, u16 opt);

void *NNS_FndCreateExpHeapEx(void *startAddress, u32 size, u16 optFlag) {
    void *endAddress = (void *)((size + (u32)startAddress) & ~3);
    startAddress = (void *)(((u32)startAddress + 3) & ~3);
    if ((u32)startAddress > (u32)endAddress || (u32)endAddress - (u32)startAddress < 0x4C) {
        return NULL;
    }
    return InitExpHeap(startAddress, endAddress, optFlag);
}
