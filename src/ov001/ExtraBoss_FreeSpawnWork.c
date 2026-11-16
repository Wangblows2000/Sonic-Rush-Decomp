#include <nitro/types.h>

extern void *gMainHeap;
u32 OS_GetArenaLo(s32 i);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
extern u8 *gExtraBossRingWork;
void ObjVram_Free(void *a, void *b);

void ExtraBoss_FreeSpawnWork(void) {
    u8 *blk;
    ObjVram_Free(*(void **)gExtraBossRingWork, *(void **)(gExtraBossRingWork + 0x34));
    ObjVram_Free(*(void **)(gExtraBossRingWork + 0x48), *(void **)(gExtraBossRingWork + 0x7c));
    blk = gExtraBossRingWork;
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
    gExtraBossRingWork = NULL;
}
