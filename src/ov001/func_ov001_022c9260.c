#include <nitro/types.h>

extern void *gMainHeap;
u32 func_02006abc(s32 i);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void func_0201cf04(void *p);
void func_0201b348(void);
void func_02020198(void *p);
void Sound_ResetHeap(void);

#pragma thumb on
void func_ov001_022c9260(u8 *p) {
    u8 *blk;
    if (*(void **)p != NULL) {
        func_0201cf04(*(void **)p);
        *(void **)p = NULL;
        func_0201b348();
        if (*(void **)(p + 4) != NULL) {
            func_02020198(p + 0x1a4);
            *(void **)(p + 4) = NULL;
        }
        Sound_ResetHeap();
    }
    blk = *(u8 **)(p + 0x1b0);
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
        Heap_Free(gMainHeap, blk);
        *(void **)(p + 0x1b0) = NULL;
    }
    *(s32 *)(p + 8) = 0;
    *(s32 *)(p + 0xc) = 0;
    *(s32 *)(p + 0x1a8) = 0;
    *(u16 *)(p + 0x1ac) = 0;
    *(u16 *)(p + 0x1ae) = 0;
}
#pragma thumb reset
