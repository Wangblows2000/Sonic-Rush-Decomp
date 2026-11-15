#include <nitro/types.h>

extern void *data_0208fe80;
u32 func_02006abc(s32 i);
u8 *func_02034a40(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
extern u8 *data_ov001_0231f084;
extern u8 *data_ov001_0231f088;
void func_0202566c(void *p);
void func_02036894(void);

void func_ov001_022f99f8(void) {
    u8 *blk;
    func_0202566c(*(void **)(data_ov001_0231f084 + 0xc));
    func_0202566c(*(void **)(data_ov001_0231f084 + 8));
    blk = data_ov001_0231f088;
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
    data_ov001_0231f088 = NULL;
    func_02036894();
}
