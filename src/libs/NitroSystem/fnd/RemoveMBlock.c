#include <nitro/types.h>

typedef struct NNSiFndExpHeapMBlockHead NNSiFndExpHeapMBlockHead;
struct NNSiFndExpHeapMBlockHead {
    u16 signature;
    u16 attribute;
    u32 blockSize;
    NNSiFndExpHeapMBlockHead *pMBHeadPrev;
    NNSiFndExpHeapMBlockHead *pMBHeadNext;
};
typedef struct { NNSiFndExpHeapMBlockHead *head; NNSiFndExpHeapMBlockHead *tail; } NNSiFndExpHeapMBlockList;

NNSiFndExpHeapMBlockHead *RemoveMBlock(NNSiFndExpHeapMBlockList *list, NNSiFndExpHeapMBlockHead *block) {
    NNSiFndExpHeapMBlockHead *const prev = block->pMBHeadPrev;
    NNSiFndExpHeapMBlockHead *const next = block->pMBHeadNext;
    if (prev) {
        prev->pMBHeadNext = next;
    } else {
        list->head = next;
    }
    if (next) {
        next->pMBHeadPrev = prev;
    } else {
        list->tail = prev;
    }
    return prev;
}
