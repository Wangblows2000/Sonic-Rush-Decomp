#include <nitro/types.h>

extern void *gMainHeap;
u32 OS_GetArenaLo(s32 i);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
extern u8 *data_ov001_0231f080;
extern u8 data_ov001_023169b4[];
void ExtraBoss_Unk_ov001_022f705c(void *p);

void ExtraBoss_Unk_ov001_022f7308(void) {
    vu16 zero;
    u8 *blk = Heap_Alloc(gMainHeap, 0x100, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, blk, 0x100);
        *(u32 *)blk = 0x100;
        *(u32 *)(blk + 0xfc) = 0x02467531;
        DC_StoreRange(blk, 0x100);
        blk += 0x20;
    }
    zero = 0;
    data_ov001_0231f080 = blk;
    MIi_CpuClear16(zero, blk, 0xcc);
    MIi_CpuCopy16(data_ov001_023169b4, data_ov001_0231f080, 0xcc);
    ExtraBoss_Unk_ov001_022f705c(data_ov001_0231f080);
}
