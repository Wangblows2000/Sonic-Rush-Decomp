#include <nitro/types.h>

extern void *data_022c4600[];
extern void *gMainHeap;
void *func_02006abc(u32 a);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

#pragma thumb on
void func_0204f540(s32 idx) {
    u8 *p;
    u32 size;
    void **slot;
    slot = &data_022c4600[idx];
    p = *slot;
    if (p != NULL) {
        if (func_02006abc(0) != p) {
            p -= 0x20;
            size = *(u32 *)p;
            if (*(u32 *)(p + size - 4) == 0x2467531) {
                vu32 v = 0xcccccccc;
                MIi_CpuClear32(v, p, size);
                DC_StoreRange(p, size);
            }
        }
        Heap_Free(gMainHeap, p);
        *slot = NULL;
    }
}
#pragma thumb reset
