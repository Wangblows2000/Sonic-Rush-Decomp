#include <nitro/types.h>

extern void *data_0208fe80;
extern void *data_ov000_0236aa8c;

void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

void func_ov000_02335d0c(void) {
    void *p = data_ov000_0236aa8c;
    u32 size;

    if (p == NULL)
        return;
    if (func_02006abc(0) != p) {
        p = (u8 *)p - 0x20;
        size = *(u32 *)p;
        if (*(u32 *)((u8 *)p + size - 4) == 0x2467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    func_020349b8(data_0208fe80, p);
    data_ov000_0236aa8c = NULL;
}
