#include <nitro/types.h>

extern void *gMainHeap;
u32 OS_GetArenaLo(s32 i);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void ZoneMap_Unk_ov001_022e8328(void);

#pragma thumb on
void ZoneMap_Unk_ov001_022e5620(u8 *p) {
    u8 *blk;
    ZoneMap_Unk_ov001_022e8328();
    blk = *(u8 **)(p + 0x50);
    if (blk != NULL) {
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
        *(void **)(p + 0x50) = NULL;
    }
}
#pragma thumb reset
