#include <nitro/types.h>

extern void *data_0208fe80;
extern u8 data_ov001_0231d9b0[];
extern u8 data_ov001_0231d9cc[];
extern u8 *data_ov001_0231f528;
extern void *data_02250458;
extern void *data_022c4484;

u8 *func_02033fbc(void *p, void *dst);
u8 *func_02034a40(void *heap, u32 size, s32 align);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
u32 func_ov001_022ffb90(u32 *p);
u32 func_ov001_022ffb84(u32 *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
u32 *func_02033a54(void *s);
void func_02033a68(void *a, void *b, u32 c);
void func_020349b8(void *heap, void *p);
void func_ov001_023029d4(void);
void func_ov001_022eeab0(void);
void func_ov001_02305368(void);
void func_ov001_02304518(void);
void func_ov001_022fdab4(void);
void func_ov001_022fd564(void);
void func_ov001_023065f8(void);
void *func_0201d9b4(void *p);
void *func_0201d9e4(void *s, u32 size, u32 a, u32 b, u32 c);
void func_02036820(void *a);

void func_ov001_022ff970(void) {
    vu32 fill1;
    vu32 fill2;
    u8 *res;
    u32 size;
    u8 *blk;

    res = func_02033fbc(data_ov001_0231d9b0, (void *)-1);
    size = ((*(u32 *)res >> 8) + 0x43) & ~0x1f;
    blk = func_02034a40(data_0208fe80, size, 0x20);
    if ((u8 *)func_02006abc(0) != blk) {
        fill1 = 0xcccccccc;
        MIi_CpuClear32(fill1, blk, size);
        *(u32 *)blk = size;
        *(u32 *)(blk + size - 4) = 0x02467531;
        DC_StoreRange(blk, size);
        blk += 0x20;
    }
    data_ov001_0231f528 = blk;
    switch (func_ov001_022ffb90((u32 *)res)) {
    case 0x10:
        func_02008648(res, blk);
        break;
    case 0x20:
        func_02008720(res, blk);
        break;
    case 0x30:
        func_020087f8(res, blk);
        break;
    default:
        func_02033a68(func_02033a54(res), blk, func_ov001_022ffb84((u32 *)res));
        break;
    }
    DC_StoreRange(blk, func_ov001_022ffb84((u32 *)res));
    if ((u8 *)func_02006abc(0) != res) {
        res -= 0x20;
        size = *(u32 *)res;
        if (*(u32 *)(res + size - 4) == 0x02467531) {
            fill2 = 0xcccccccc;
            MIi_CpuClear32(fill2, res, size);
            DC_StoreRange(res, size);
        }
    }
    func_020349b8(data_0208fe80, res);
    func_ov001_023029d4();
    func_ov001_022eeab0();
    func_ov001_02305368();
    func_ov001_02304518();
    func_ov001_022fdab4();
    func_ov001_022fd564();
    func_ov001_023065f8();
    data_022c4484 = func_0201d9b4(data_02250458);
    data_022c4484 = func_0201d9e4(data_02250458, 0x60400, 0, 0, 0);
    func_02033fbc(data_ov001_0231d9cc, data_022c4484);
    func_02036820(data_022c4484);
}
