#include <nitro/types.h>

extern u8 data_0207a208[];
extern void *data_0208fe80;
extern u8 *data_ov000_0236ac28;
typedef struct { s32 unk0; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;

void func_ov000_0234dee0(void);
void func_ov000_0234dfb8(void);
u8 *func_02033fbc(void *path, s32 b);
u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
s32 func_0204eaf8(void *p);
u32 func_0204eaec(void *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *func_02033a54(void *p);
void func_02033a68(void *src, void *dst, u32 size);
void func_0205453c(s32 a, s32 b);
void func_0204525c(void);

static inline u8 *Alloc_0204e934(u32 size) {
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

static inline void Free_0204e934(u8 *blk) {
    if ((u8 *)func_02006abc(0) != blk) {
        u32 size;
        blk -= 0x20;
        size = *(u32 *)blk;
        if (*(u32 *)(blk + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, blk, size);
            DC_StoreRange(blk, size);
        }
    }
    func_020349b8(data_0208fe80, blk);
}

void func_0204e934(void) {
    u8 *f;
    u8 *p;
    u32 size;

    func_ov000_0234dee0();
    f = func_02033fbc(data_0207a208, -1);
    size = ((*(u32 *)f >> 8) + 0x43) & ~0x1f;
    p = Alloc_0204e934(size);
    data_ov000_0236ac28 = p;
    switch (func_0204eaf8(f)) {
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
        func_02033a68(func_02033a54(f), p, func_0204eaec(f));
        break;
    }
    DC_StoreRange(p, func_0204eaec(f));
    Free_0204e934(f);
    func_ov000_0234dfb8();
    func_0205453c(1, data_022c4560.unk0);
    func_0204525c();
}
