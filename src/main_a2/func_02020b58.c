#include <nitro/types.h>
#include <main_a2/expheap.h>

void *func_02020ce4(UnkExpHeap *eh, UnkMBlock *freeBlock, void *mem, u32 size, u16 dir);

void *func_02020b58(UnkHeapHead *heap, u32 size, s32 align) {
    UnkExpHeap *eh;
    BOOL first;
    UnkMBlock *found;
    UnkMBlock *b;
    u32 foundSize;
    void *foundMem;
    eh = &heap->exp;
    first = (u16)(eh->feature & 1) == 0;
    found = NULL;
    foundSize = 0xFFFFFFFF;
    foundMem = NULL;
    for (b = eh->freeList.tail; b != NULL; b = b->prev) {
        void *mem = b + 1;
        void *end = (void *)(b->blockSize + (u32)mem);
        void *addr = (void *)(((u32)end - size) & ~(align - 1));
        if ((s32)((u32)addr - (u32)mem) >= 0) {
            if (foundSize > b->blockSize) {
                found = b;
                foundSize = b->blockSize;
                foundMem = addr;
                if (first || foundSize == size) break;
            }
        }
    }
    if (found == NULL) return NULL;
    return func_02020ce4(eh, found, foundMem, size, 1);
}
