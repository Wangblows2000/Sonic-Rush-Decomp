#include <nitro/types.h>

u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

extern void *data_0208fe80;
extern u8 *data_ov000_02369ad0[];

static inline void Free_02049eb4(u8 *p) {
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

void func_02049eb4(void) {
    u32 i;

    for (i = 0; i < 4; i++) {
        if (data_ov000_02369ad0[i] != NULL) {
            Free_02049eb4(data_ov000_02369ad0[i]);
            data_ov000_02369ad0[i] = NULL;
        }
    }
}
