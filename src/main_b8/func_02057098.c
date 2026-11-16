#include <nitro/types.h>

extern void *data_0208fe80;

u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

#pragma thumb on
BOOL func_02057098(u8 *p) {
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
        func_020349b8(data_0208fe80, p);
    }
    return TRUE;
}
#pragma thumb reset
