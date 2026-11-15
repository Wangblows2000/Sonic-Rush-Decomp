#include <nitro/types.h>

extern u8 *data_ov001_0231f530;
extern void *data_0208fe80;
u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);

void func_ov001_02301728(void) {
    vu16 zero;
    u8 *blk = func_02034a40(data_0208fe80, 0x1a0, 0x20);
    if ((u8 *)func_02006abc(0) != blk) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, blk, 0x1a0);
        *(u32 *)blk = 0x1a0;
        *(u32 *)(blk + 0x19c) = 0x02467531;
        DC_StoreRange(blk, 0x1a0);
        blk += 0x20;
    }
    zero = 0;
    data_ov001_0231f530 = blk;
    MIi_CpuClear16(zero, blk, 0x174);
}
