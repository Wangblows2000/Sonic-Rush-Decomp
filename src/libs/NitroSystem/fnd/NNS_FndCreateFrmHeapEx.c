#include <nitro/types.h>

void *InitFrameHeap(void *start, void *end, u16 opt);

void *NNS_FndCreateFrmHeapEx(void *startAddress, u32 size, u16 optFlag) {
    void *endAddress = (void *)((size + (u32)startAddress) & ~3);
    startAddress = (void *)(((u32)startAddress + 3) & ~3);
    if ((u32)startAddress > (u32)endAddress || (u32)endAddress - (u32)startAddress < 0x30) {
        return NULL;
    }
    return InitFrameHeap(startAddress, endAddress, optFlag);
}
