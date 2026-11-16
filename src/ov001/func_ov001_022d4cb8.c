#include <nitro/types.h>

extern void *gMainHeap;
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

#pragma thumb on
void func_ov001_022d4cb8(u8 *p) {
    s32 i;
    void *vnull = NULL;
    s32 zero = 0;
    for (i = 0; i < 4; i++) {
        u8 *blk = *(u8 **)(p + 0x15c);
        if (blk != NULL) {
            if ((u8 *)func_02006abc(zero) != blk) {
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
            *(void **)(p + 0x15c) = vnull;
        }
        p += 4;
    }
}
#pragma thumb reset
