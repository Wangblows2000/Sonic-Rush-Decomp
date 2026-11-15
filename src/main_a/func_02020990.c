#include <nitro/types.h>

typedef struct NNSiFndExpHeapMBlockHead NNSiFndExpHeapMBlockHead;
struct NNSiFndExpHeapMBlockHead { u16 signature; u16 attribute; u32 blockSize; NNSiFndExpHeapMBlockHead *prev; NNSiFndExpHeapMBlockHead *next; };
typedef struct { NNSiFndExpHeapMBlockHead *head; NNSiFndExpHeapMBlockHead *tail; } NNSiFndExpHeapMBlockList;
typedef struct { NNSiFndExpHeapMBlockList freeList; NNSiFndExpHeapMBlockList usedList; } NNSiFndExpHeapHead;
typedef struct { void *start; void *end; } NNSiMemRegion;

void GetRegionOfMBlock(NNSiMemRegion *region, NNSiFndExpHeapMBlockHead *block);
NNSiFndExpHeapMBlockHead *RemoveMBlock(NNSiFndExpHeapMBlockList *list, NNSiFndExpHeapMBlockHead *block);
BOOL func_02020a60(NNSiFndExpHeapHead *eh, NNSiMemRegion *region);

void func_02020990(u8 *heap, void *memBlock) {
    NNSiFndExpHeapHead *eh = (NNSiFndExpHeapHead *)(heap + 0x24);
    NNSiFndExpHeapMBlockHead *block = (NNSiFndExpHeapMBlockHead *)((u8 *)memBlock - 0x10);
    NNSiMemRegion region;
    GetRegionOfMBlock(&region, block);
    RemoveMBlock(&eh->usedList, block);
    func_02020a60(eh, &region);
}
