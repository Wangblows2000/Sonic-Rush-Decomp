#include <nitro/types.h>
#include <main_a2/expheap.h>

void *AllocUsedBlockFromFreeBlock(UnkExpHeap *eh, UnkMBlock *freeBlock, void *mem, u32 size, u16 dir);

void *AllocFromHead(UnkHeapHead *heap, u32 size, s32 align) {
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
    for (b = eh->freeList.head; b != NULL; b = b->next) {
        void *mem = b + 1;
        void *addr = (void *)(((align - 1) + (u32)mem) & ~(align - 1));
        u32 offset = (u32)addr - (u32)mem;
        if (b->blockSize >= size + offset) {
            if (foundSize > b->blockSize) {
                found = b;
                foundSize = b->blockSize;
                foundMem = addr;
                if (first || foundSize == size) break;
            }
        }
    }
    if (found == NULL) return NULL;
    return AllocUsedBlockFromFreeBlock(eh, found, foundMem, size, 0);
}
