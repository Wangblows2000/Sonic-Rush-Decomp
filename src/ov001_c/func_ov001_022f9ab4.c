#include <nitro/types.h>

typedef struct UnkStruct_022f9ab4_arc {
    u8 pad[0x60];
} UnkStruct_022f9ab4_arc;

typedef struct UnkStruct_022f9ab4 {
    u32 unk0;
    u32 unk4;
    u32 res[4];
} UnkStruct_022f9ab4;

extern void *data_0208fe80;
extern u8 data_ov001_0231d5a0[];
extern char data_ov001_0231d5bc[];
extern u8 data_ov001_0231d5c0[];
extern u8 *data_ov001_0231f088;
extern UnkStruct_022f9ab4 *data_ov001_0231f084;
extern void *data_02250458;

u8 *func_02033fbc(void *p, s32 v);
u8 *func_02034a40(void *heap, u32 size, s32 align);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
u32 func_ov001_022f9d14(u32 *p);
u32 func_ov001_022f9d08(u32 *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
u32 *func_02033a54(void *s);
void func_02033a68(void *a, void *b, u32 c);
void func_020349b8(void *heap, void *p);
BOOL func_02021310(void *arc, const char *name, void *narc);
u32 func_02021264(void *arc, u32 idx);
void func_020257f8(u32 p);
BOOL func_020212e0(void *arc);
void func_02036854(void *a);
void func_0201e7cc(s32 a, void *b);

void func_ov001_022f9ab4(void) {
    vu32 fill1;
    vu32 fill2;
    UnkStruct_022f9ab4_arc arc;
    u8 *res;
    u32 size;
    u8 *blk;
    s32 i;

    res = func_02033fbc(data_ov001_0231d5a0, -1);
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
    data_ov001_0231f088 = blk;
    switch (func_ov001_022f9d14((u32 *)res)) {
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
        func_02033a68(func_02033a54(res), blk, func_ov001_022f9d08((u32 *)res));
        break;
    }
    DC_StoreRange(blk, func_ov001_022f9d08((u32 *)res));
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
    func_02021310(&arc, data_ov001_0231d5bc, data_ov001_0231f088);
    data_ov001_0231f084->unk0 = func_02021264(&arc, 6);
    data_ov001_0231f084->unk4 = func_02021264(&arc, 5);
    for (i = 0; i < 4; i++)
        data_ov001_0231f084->res[i] = func_02021264(&arc, i + 7);
    func_020257f8(data_ov001_0231f084->res[1]);
    func_020257f8(data_ov001_0231f084->res[0]);
    func_020212e0(&arc);
    func_02036854(data_ov001_0231d5c0);
    func_0201e7cc(0xa, data_02250458);
}
