#include <nitro/types.h>

extern void *gMainHeap;
u32 func_02006abc(s32 i);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void Model_Release(void *p);
void func_0202566c(void *p);

void func_ov001_022df008(void *a, u8 *p) {
    u8 *blk;
    Model_Release(p + 0x5c);
    func_0202566c(*(void **)(p + 0xc));
    blk = *(u8 **)(p + 8);
    if (blk == NULL) {
        return;
    }
    {
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
        Heap_Free(gMainHeap, blk);
    }
    *(void **)(p + 8) = NULL;
}
