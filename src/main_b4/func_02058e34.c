#include <nitro/types.h>

extern void *data_0208fe78;
extern u32 data_022c4680;

u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
u32 func_020588bc(void *p);
BOOL func_02058ab0(u8 *p);
BOOL func_020583f4(void *a, u8 *p);

static inline u8 *Alloc_02058e34(u32 size) {
    u8 *p = func_02034a40(data_0208fe78, size, 0x20);
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

static inline void Free_02058e34(u8 *p) {
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
    func_020349b8(data_0208fe78, p);
}

BOOL func_02058e34(void) {
    u8 *p;

    p = Alloc_02058e34(0x340);
    if (p == (u8 *)func_02006abc(0))
        return FALSE;
    {
        vu32 zero = 0;
        MIi_CpuClear32(zero, p, 0x314);
    }
    data_022c4680 = func_020588bc(&data_022c4680);
    if (func_02058ab0(p)) {
        Free_02058e34(p);
        return TRUE;
    }
    if (func_020583f4(&data_022c4680, p)) {
        Free_02058e34(p);
        return FALSE;
    }
    Free_02058e34(p);
    return TRUE;
}
