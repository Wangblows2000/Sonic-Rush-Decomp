#include <nitro/types.h>

void *NNS_FndGetNextListObject(void *list, void *obj);

typedef struct { u8 unk0[0xC]; u8 childList[0xC]; u32 heapStart; u32 heapEnd; } UnkHeap_020208f0;

UnkHeap_020208f0 *FindContainHeap(void *list, u32 addr) {
    UnkHeap_020208f0 *heap = NULL;
    while ((heap = NNS_FndGetNextListObject(list, heap)) != NULL) {
        if (heap->heapStart <= addr && addr < heap->heapEnd) {
            UnkHeap_020208f0 *child = FindContainHeap(heap->childList, addr);
            if (child == NULL) child = heap;
            return child;
        }
    }
    return NULL;
}
