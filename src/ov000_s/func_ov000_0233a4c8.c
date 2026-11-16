#include <nitro/types.h>

extern void *data_0208fe80;
extern void *data_ov000_0236aabc[];

void *func_02021264(void *arc, s32 idx);
void *func_02034a40(void *heap, u32 size, s32 align);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
u32 func_ov000_0233ac84(void *res);
u32 func_ov000_0233ac78(void *res);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *func_02033a54(void *res);
void func_02033a68(void *src, void *dst, u32 size);

static inline void *UnkAlloc_0233a4c8(u32 size, s32 align) {
    u8 *p;
    size = (size + 0x43) & ~0x1f;
    p = func_02034a40(data_0208fe80, size, align);
    if (func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x2467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

static inline void Fill_0233a4c8(void *p, u32 size) {
    vu32 fill = 0xcccccccc;
    MIi_CpuClear32(fill, p, size);
}

void func_ov000_0233a4c8(void *arc) {
    u32 size;
    void *res;
    u8 *dst;

    res = func_02021264(arc, 4);
    dst = UnkAlloc_0233a4c8(*(u32 *)res >> 8, 0x20);
    data_ov000_0236aabc[0] = dst;
    switch (func_ov000_0233ac84(res)) {
    case 0x10:
        func_02008648(res, dst);
        break;
    case 0x20:
        func_02008720(res, dst);
        break;
    case 0x30:
        func_020087f8(res, dst);
        break;
    default:
        func_02033a68(func_02033a54(res), dst, func_ov000_0233ac78(res));
        break;
    }
    DC_StoreRange(dst, func_ov000_0233ac78(res));
    res = func_02021264(arc, 0);
    dst = UnkAlloc_0233a4c8(*(u32 *)res >> 8, 0x20);
    data_ov000_0236aabc[1] = dst;
    switch (func_ov000_0233ac84(res)) {
    case 0x10:
        func_02008648(res, dst);
        break;
    case 0x20:
        func_02008720(res, dst);
        break;
    case 0x30:
        func_020087f8(res, dst);
        break;
    default:
        func_02033a68(func_02033a54(res), dst, func_ov000_0233ac78(res));
        break;
    }
    DC_StoreRange(dst, func_ov000_0233ac78(res));
    res = func_02021264(arc, 5);
    dst = UnkAlloc_0233a4c8(*(u32 *)res >> 8, 0x20);
    data_ov000_0236aabc[2] = dst;
    switch (func_ov000_0233ac84(res)) {
    case 0x10:
        func_02008648(res, dst);
        break;
    case 0x20:
        func_02008720(res, dst);
        break;
    case 0x30:
        func_020087f8(res, dst);
        break;
    default:
        func_02033a68(func_02033a54(res), dst, func_ov000_0233ac78(res));
        break;
    }
    DC_StoreRange(dst, func_ov000_0233ac78(res));
    res = func_02021264(arc, 1);
    dst = UnkAlloc_0233a4c8(*(u32 *)res >> 8, 0x20);
    data_ov000_0236aabc[3] = dst;
    switch (func_ov000_0233ac84(res)) {
    case 0x10:
        func_02008648(res, dst);
        break;
    case 0x20:
        func_02008720(res, dst);
        break;
    case 0x30:
        func_020087f8(res, dst);
        break;
    default:
        func_02033a68(func_02033a54(res), dst, func_ov000_0233ac78(res));
        break;
    }
    DC_StoreRange(dst, func_ov000_0233ac78(res));
    res = func_02021264(arc, 7);
    dst = UnkAlloc_0233a4c8(*(u32 *)res >> 8, 0x20);
    data_ov000_0236aabc[4] = dst;
    switch (func_ov000_0233ac84(res)) {
    case 0x10:
        func_02008648(res, dst);
        break;
    case 0x20:
        func_02008720(res, dst);
        break;
    case 0x30:
        func_020087f8(res, dst);
        break;
    default:
        func_02033a68(func_02033a54(res), dst, func_ov000_0233ac78(res));
        break;
    }
    DC_StoreRange(dst, func_ov000_0233ac78(res));
    res = func_02021264(arc, 8);
    dst = UnkAlloc_0233a4c8(*(u32 *)res >> 8, 0x20);
    data_ov000_0236aabc[5] = dst;
    switch (func_ov000_0233ac84(res)) {
    case 0x10:
        func_02008648(res, dst);
        break;
    case 0x20:
        func_02008720(res, dst);
        break;
    case 0x30:
        func_020087f8(res, dst);
        break;
    default:
        func_02033a68(func_02033a54(res), dst, func_ov000_0233ac78(res));
        break;
    }
    DC_StoreRange(dst, func_ov000_0233ac78(res));
    res = func_02021264(arc, 2);
    size = ((*(u32 *)res >> 8) + 0x43) & ~0x1f;
    dst = func_02034a40(data_0208fe80, size, 0x20);
    if ((u8 *)func_02006abc(0) != dst) {
        Fill_0233a4c8(dst, size);
        *(u32 *)dst = size;
        *(u32 *)((u8 *)dst + size - 4) = 0x02467531;
        DC_StoreRange(dst, size);
        dst = (u8 *)dst + 0x20;
    }
    data_ov000_0236aabc[6] = dst;
    switch (func_ov000_0233ac84(res)) {
    case 0x10:
        func_02008648(res, dst);
        break;
    case 0x20:
        func_02008720(res, dst);
        break;
    case 0x30:
        func_020087f8(res, dst);
        break;
    default:
        func_02033a68(func_02033a54(res), dst, func_ov000_0233ac78(res));
        break;
    }
    DC_StoreRange(dst, func_ov000_0233ac78(res));
}
