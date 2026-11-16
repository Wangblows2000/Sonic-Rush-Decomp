#include <nitro/types.h>

extern void *data_022c4600[];
extern void *data_0208fe80;
void *func_02006abc(u32 a);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

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
        func_020349b8(data_0208fe80, p);
        *slot = NULL;
    }
}
#pragma thumb reset
