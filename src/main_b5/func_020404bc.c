#include <nitro/types.h>

u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

extern void *data_0208fe78;

static inline u8 *Alloc_020404bc(u32 sz) {
    u32 size = sz;
    u8 *p = func_02034a40(data_0208fe78, size, -0x20);
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

static inline void Free_020404bc(u8 *p) {
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

u8 *func_020404bc(void *unused, u32 size, u8 *p) {
    if (p == NULL)
        return Alloc_020404bc((size + 0x43) & ~0x1f);
    Free_020404bc(p);
    return NULL;
}
