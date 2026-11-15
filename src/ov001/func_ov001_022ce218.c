#include <nitro/types.h>

extern void *data_0208fe80;
u32 func_02006abc(s32 i);
u8 *func_02034a40(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
extern u8 data_022503e0[];
void func_02037d40(void);
void func_020351d8(void *p, s32 a, s32 b);
void func_ov001_022ce3a0(void *p);
void func_ov001_022ce304(void);

#pragma thumb on
void func_ov001_022ce218(u8 *t) {
    u8 *p = *(u8 **)(t + 0x10);
    u8 *blk = *(u8 **)(p + 0x1e04);
    if (blk != NULL) {
        if ((u8 *)func_02006abc(0) != blk) {
            u32 size;
            blk -= 0x20;
            size = *(u32 *)blk;
            if (*(u32 *)(blk + size - 4) == 0x02467531) {
                vu32 fill = 0xcccccccc;
                MIi_CpuClear32(fill, blk, size);
                DC_StoreRange(blk, size);
            }
        }
        func_020349b8(data_0208fe80, blk);
        *(void **)(p + 0x1e04) = NULL;
    }
    func_02037d40();
    func_020351d8(data_022503e0, 0, 0);
    func_ov001_022ce3a0(p);
    func_ov001_022ce304();
}
#pragma thumb reset
