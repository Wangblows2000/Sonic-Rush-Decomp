#include <nitro/types.h>

extern void *gMainHeap;

u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

#pragma thumb on
BOOL func_02060480(u8 *p) {
    if (p != NULL) {
        if ((u8 *)func_02006abc(0) != p) {
            u32 size;
            p -= 0x20;
            size = *(u32 *)p;
            if (*(u32 *)(p + size - 4) == 0x02467531) {
                vu32 fill = 0xcccccccc;
                MIi_CpuClear32(fill, p, size);
                DC_StoreRange(p, size);
            }
        }
        Heap_Free(gMainHeap, p);
    }
    return TRUE;
}
#pragma thumb reset
