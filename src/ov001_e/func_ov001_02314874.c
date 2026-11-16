#include <nitro/types.h>

extern void *data_0208fe80;
extern void *data_ov001_0231f6a0;

u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);
void func_ov001_023145d8(void);
void func_02037d60(void);

static inline u8 *Alloc(u32 sz) {
    u32 size = sz;
    u8 *p = func_02034a40(data_0208fe80, size, 0x20);
    if ((u8 *)func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x02467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

void func_ov001_02314874(void) {
    u8 *p = Alloc(0x140);

    data_ov001_0231f6a0 = p;
    MI_CpuFill8(p, 0, 0x114);
    func_ov001_023145d8();
    func_02037d60();
}
