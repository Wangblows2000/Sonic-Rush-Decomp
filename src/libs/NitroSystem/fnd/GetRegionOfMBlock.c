#include <nitro/types.h>

typedef struct NNSiFndExpHeapMBlockHead NNSiFndExpHeapMBlockHead;
struct NNSiFndExpHeapMBlockHead {
    u16 signature;
    u16 attribute;
    u32 blockSize;
    NNSiFndExpHeapMBlockHead *pMBHeadPrev;
    NNSiFndExpHeapMBlockHead *pMBHeadNext;
};
typedef struct { void *start; void *end; } NNSiMemRegion;

void GetRegionOfMBlock(NNSiMemRegion *region, NNSiFndExpHeapMBlockHead *block) {
    region->start = (u8 *)block - (u16)((block->attribute >> 8) & 0x7F);
    region->end = (void *)(block->blockSize + (u32)(block + 1));
}
