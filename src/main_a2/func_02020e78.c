#include <nitro/types.h>
#include <main_a2/expheap.h>

void func_02020834(UnkHeapHead *heap, u32 signature, void *start, void *end, u16 opt);
UnkMBlock *InitMBlock(const UnkMemRegion *region, u16 signature);

UnkHeapHead *func_02020e78(UnkHeapHead *heap, void *end, u16 opt) {
    UnkExpHeap *eh = &heap->exp;
    UnkMemRegion region;
    UnkMBlock *b;
    func_02020834(heap, 0x45585048, eh + 1, end, opt);
    eh->groupID = 0;
    eh->feature = 0;
    UNK_SET_BITS(eh->feature, 0, 1, 0);
    region.start = heap->heapStart;
    region.end = heap->heapEnd;
    b = InitMBlock(&region, 0x4652);
    heap->exp.freeList.head = b;
    eh->freeList.tail = b;
    eh->usedList.head = NULL;
    eh->usedList.tail = NULL;
    return heap;
}
