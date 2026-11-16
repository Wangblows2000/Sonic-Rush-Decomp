#include <nitro/types.h>

extern u8 *data_ov001_0231f094;
extern void *gMainHeap;
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);

void ExtraBoss_Unk_ov001_022fd6a4(void) {
    vu16 zero;
    u8 *blk = Heap_Alloc(gMainHeap, 0x40, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, blk, 0x40);
        *(u32 *)blk = 0x40;
        *(u32 *)(blk + 0x3c) = 0x02467531;
        DC_StoreRange(blk, 0x40);
        blk += 0x20;
    }
    zero = 0;
    data_ov001_0231f094 = blk;
    MIi_CpuClear16(zero, blk, 2);
}
