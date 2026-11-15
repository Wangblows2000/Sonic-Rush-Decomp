#include <nitro/types.h>

typedef struct { u8 pad[0xc]; void *heap; u8 *mem; } UnkStruct_022c61c8;
extern void *data_0208fe80;
void func_0201b348(void);
void func_0201da54(void *h);
void func_0201db14(void *h);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
void func_02036894(void);

void func_ov001_022c61c8(UnkStruct_022c61c8 *p) {
    u8 *blk;
    func_0201b348();
    func_0201da54(p->heap);
    func_0201db14(p->heap);
    blk = p->mem;
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
    func_02036894();
}
