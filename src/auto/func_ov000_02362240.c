/* paramclone of TryAgain_ReleaseAssets (src/ov001/TryAgain_ReleaseAssets.c); immediates and relocations remapped. */
#include <nitro/types.h>

extern void *gSystemHeap;
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

#pragma thumb on
void func_ov000_02362240(u8 *p) {
    s32 i;
    void *vnull = NULL;
    s32 zero = 0;
    for (i = 0; i < 11; i++) {
        u8 *blk = *(u8 **)(p + 0x1010);
        if (blk != NULL) {
            if ((u8 *)OS_GetArenaLo(zero) != blk) {
                u32 size;
                blk -= 0x20;
                size = *(u32 *)blk;
                if (*(u32 *)(blk + size - 4) == 0x02467531) {
                    vu32 fill = 0xcccccccc;
                    MIi_CpuClear32(fill, blk, size);
                    DC_StoreRange(blk, size);
                }
            }
            Heap_Free(gSystemHeap, blk);
        }
        *(void **)(p + 0x1010) = vnull;
        p += 4;
    }
}
#pragma thumb reset
