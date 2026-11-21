#include <nitro/types.h>
#include <NitroSystem/expheap.h>

void GetRegionOfMBlock(UnkMemRegion *region, UnkMBlock *block);
UnkMBlock *RemoveMBlock(UnkMBlockList *list, UnkMBlock *block);
UnkMBlock *InitMBlock(const UnkMemRegion *region, u16 signature);
UnkMBlock *InsertMBlock(UnkMBlockList *list, UnkMBlock *target, UnkMBlock *prev);
void MIi_CpuClear32(u32 data, void *dst, u32 size);

#define SET_BITS_02020ce4(data, st, bits, val) \
    do { \
        u32 maskBits = (u32)((1 << (bits)) - 1); \
        u32 newVal = (val) & maskBits; \
        maskBits <<= (st); \
        (data) &= ~maskBits; \
        (data) |= newVal << (st); \
    } while (0)

static inline void FillAlloc_02020ce4(UnkHeapHead *heap, void *addr, u32 size)
{
    if ((u16)(heap->attribute & 0xff) & 1) {
        vu32 zero = 0;
        MIi_CpuClear32(zero, addr, size);
    }
}

void *AllocUsedBlockFromFreeBlock(UnkExpHeap *exp, UnkMBlock *free, void *mem, u32 size, u16 dir)
{
    UnkMemRegion freeRgnT;
    UnkMemRegion freeRgnB;
    UnkMemRegion usedRgn;
    UnkMBlock *prev;

    GetRegionOfMBlock(&freeRgnT, free);
    freeRgnB.end = freeRgnT.end;
    freeRgnB.start = (void *)(size + (u32)mem);
    freeRgnT.end = (u8 *)mem - sizeof(UnkMBlock);
    prev = RemoveMBlock(&exp->freeList, free);
    if ((u32)((u8 *)freeRgnT.end - (u8 *)freeRgnT.start) < sizeof(UnkMBlock)) {
        freeRgnT.end = freeRgnT.start;
    } else {
        prev = InsertMBlock(&exp->freeList, InitMBlock(&freeRgnT, 0x4652), prev);
    }
    if ((u32)((u8 *)freeRgnB.end - (u8 *)freeRgnB.start) < sizeof(UnkMBlock)) {
        freeRgnB.start = freeRgnB.end;
    } else {
        InsertMBlock(&exp->freeList, InitMBlock(&freeRgnB, 0x4652), prev);
    }
    FillAlloc_02020ce4((UnkHeapHead *)((u8 *)exp - 0x24), freeRgnT.end, (u8 *)freeRgnB.start - (u8 *)freeRgnT.end);
    {
        UnkMBlock *blk;
        usedRgn.start = (u8 *)mem - sizeof(UnkMBlock);
        usedRgn.end = freeRgnB.start;
        blk = InitMBlock(&usedRgn, 0x5544);
        UnkSetBits(&blk->attribute, 15, 1, dir);
        UnkSetBits(&blk->attribute, 8, 7, (u16)((u8 *)blk - (u8 *)freeRgnT.end));
        SET_BITS_02020ce4(blk->attribute, 0, 8, exp->groupID);
        InsertMBlock(&exp->usedList, blk, exp->usedList.tail);
    }
    return mem;
}
