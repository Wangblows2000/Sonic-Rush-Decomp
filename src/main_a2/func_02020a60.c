#include <nitro/types.h>
#include <main_a2/expheap.h>

UnkMBlock *RemoveMBlock(UnkMBlockList *list, UnkMBlock *block);
UnkMBlock *InitMBlock(const UnkMemRegion *region, u16 signature);
UnkMBlock *InsertMBlock(UnkMBlockList *list, UnkMBlock *target, UnkMBlock *prev);

BOOL func_02020a60(UnkMBlockList *freeList, const UnkMemRegion *region) {
    UnkMBlock *prev = NULL;
    UnkMemRegion rgn = *region;
    UnkMBlock *b;
    for (b = freeList->head; b != NULL; b = b->next) {
        if ((void *)b < region->start) {
            prev = b;
            continue;
        }
        if ((void *)b == region->end) {
            rgn.end = (void *)(b->blockSize + (u32)(b + 1));
            RemoveMBlock(freeList, b);
        }
        break;
    }
    if (prev != NULL && (void *)(prev->blockSize + (u32)(prev + 1)) == region->start) {
        rgn.start = prev;
        prev = RemoveMBlock(freeList, prev);
    }
    if ((u32)((u8 *)rgn.end - (u8 *)rgn.start) < sizeof(UnkMBlock)) return FALSE;
    InsertMBlock(freeList, InitMBlock(&rgn, 0x4652), prev);
    return TRUE;
}
