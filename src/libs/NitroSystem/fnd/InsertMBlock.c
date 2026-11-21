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

NNSiFndExpHeapMBlockHead *InsertMBlock(NNSiFndExpHeapMBlockList *list, NNSiFndExpHeapMBlockHead *target, NNSiFndExpHeapMBlockHead *prev) {
    NNSiFndExpHeapMBlockHead *next;
    target->pMBHeadPrev = prev;
    if (prev) {
        next = prev->pMBHeadNext;
        prev->pMBHeadNext = target;
    } else {
        next = list->head;
        list->head = target;
    }
    target->pMBHeadNext = next;
    if (next) {
        next->pMBHeadPrev = target;
    } else {
        list->tail = target;
    }
    return target;
}
