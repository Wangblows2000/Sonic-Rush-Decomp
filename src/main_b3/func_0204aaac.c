#include <nitro/types.h>

typedef struct { u8 pad0[0x5c]; u8 *file; } UnkWork_0204aaac;
extern void *data_0208fe80;
extern u8 *data_022c4488;

u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
s32 func_0204abec(void *p);
u32 func_0204abe0(void *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *func_02033a54(void *p);
void func_02033a68(void *src, void *dst, u32 size);

void func_0204aaac(UnkWork_0204aaac *w) {
    u8 *f;
    u8 *p;
    u32 size;

    size = ((*(u32 *)w->file >> 8) + 0x43) & ~0x1f;
    p = func_02034a40(data_0208fe80, size, 0x20);
    if ((u8 *)func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x02467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    data_022c4488 = p;
    f = w->file;
    switch (func_0204abec(f)) {
    case 0x10:
        func_02008648(f, p);
        break;
    case 0x20:
        func_02008720(f, p);
        break;
    case 0x30:
        func_020087f8(f, p);
        break;
    default:
        func_02033a68(func_02033a54(f), p, func_0204abe0(f));
        break;
    }
    DC_StoreRange(p, func_0204abe0(f));
}
