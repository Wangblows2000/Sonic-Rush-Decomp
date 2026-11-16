#include <nitro/types.h>

extern void *data_ov001_0231f6a0;
extern void *gMainHeap;

void Touch_Disable(void);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

static inline void Free(void *p) {
    u8 *blk = p;
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
}

void func_ov001_023147dc(void) {
    Free(data_ov001_0231f6a0);
    data_ov001_0231f6a0 = NULL;
    Touch_Disable();
}
