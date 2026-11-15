/* paramclone of func_ov001_022d6684 (src/ov001/func_ov001_022d6684.c); immediates and relocations remapped. */
#include <nitro/types.h>

extern void *data_0208fe80;
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

#pragma thumb on
void func_ov000_02359c60(u8 *p) {
    s32 i;
    void *vnull = NULL;
    s32 zero = 0;
    for (i = 0; i < 15; i++) {
        u8 *blk = *(u8 **)(p + 0xcf4);
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
            func_020349b8(data_0208fe80, blk);
            *(void **)(p + 0xcf4) = vnull;
        }
        p += 4;
    }
}
#pragma thumb reset
