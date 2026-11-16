#include <nitro/types.h>

extern void *gMainHeap;
u32 OS_GetArenaLo(s32 i);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
extern u8 *gSpecialStageData;
void func_ov001_0231414c(void *p);

void SpecialStage_ReleaseStageData(void) {
    u8 *blk = gSpecialStageData;
    if (blk == NULL) {
        return;
    }
    if (*(void **)(blk + 0x14) != NULL) {
        func_ov001_0231414c(*(void **)(blk + 0x14));
        *(void **)(blk + 0x14) = NULL;
    }
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        u32 size;
        blk -= 0x20;
        size = *(u32 *)blk;
        if (*(u32 *)(blk + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, blk, size);
            DC_StoreRange(blk, size);
        }
    }
    Heap_Free(gMainHeap, blk);
    gSpecialStageData = NULL;
}
