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

NNSiFndExpHeapMBlockHead *InitMBlock(const NNSiMemRegion *region, u16 signature) {
    NNSiFndExpHeapMBlockHead *block = region->start;
    block->signature = signature;
    block->attribute = 0;
    block->blockSize = (u8 *)region->end - (u8 *)(block + 1);
    block->pMBHeadPrev = NULL;
    block->pMBHeadNext = NULL;
    return block;
}
