#include <nitro/types.h>

typedef struct UnkStruct_0230be84_o {
    void *res[7];
    void *res2[4];
    void *unk2C;
} UnkStruct_0230be84_o;

typedef struct UnkStruct_0230be84 {
    u8 pad0[0x1d0];
    void *heap;
    u8 *mem;
} UnkStruct_0230be84;

extern u8 data_02072cb8;
extern UnkStruct_0230be84 *data_ov001_0231f5c0;
extern char **data_ov001_0231e650[];
extern char **data_ov001_0231e670[];
extern void *data_ov001_0231df98;
extern void *data_0208fe80;
extern u8 data_ov001_0231f5c8[];
extern char data_ov001_0231e6b4[];
extern u8 data_ov001_0231e6cc[];
extern void *data_02250458;

void *func_02033fbc(void *p, s32 v);
void *func_ov001_0230b154(void *p);
u8 *func_02034a40(void *heap, u32 size, s32 align);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void MI_CpuFill8(void *dst, u8 v, u32 size);
void *func_0201db30(void *p, u32 size);
void func_0201d868(void *s, const char *path, void *a, u32 b);
void func_0201ebec(void *h);
void func_0201e794(u32 a, void *b);
void func_0201e75c(u32 a, void *b);
void func_02036854(void *a);
void func_0201e7cc(s32 a, void *b);

static inline u32 GetLang(void) {
    switch (data_02072cb8) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return data_02072cb8;
    default:
        return 1;
    }
}

void func_ov001_0230be84(UnkStruct_0230be84_o *o) {
    UnkStruct_0230be84 *g = data_ov001_0231f5c0;
    s32 i;
    char **a = data_ov001_0231e650[GetLang()];
    char **b = data_ov001_0231e670[GetLang()];
    u8 *blk;

    for (i = 0; i < 7; i++)
        o->res[i] = func_02033fbc(a[i], -1);
    for (i = 0; i < 4; i++)
        o->res2[i] = func_02033fbc(b[i], -1);
    o->unk2C = func_ov001_0230b154(data_ov001_0231df98);
    blk = func_02034a40(data_0208fe80, 0x69040, 0x20);
    if ((u8 *)func_02006abc(0) != blk) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, blk, 0x69040);
        *(u32 *)blk = 0x69040;
        *(u32 *)(blk + 0x6903c) = 0x02467531;
        DC_StoreRange(blk, 0x69040);
        blk += 0x20;
    }
    g->mem = blk;
    MI_CpuFill8(g->mem, 0, 0x69000);
    g->heap = func_0201db30(g->mem, 0x69000);
    func_0201d868(data_ov001_0231f5c8, data_ov001_0231e6b4, g->heap, 0);
    func_0201ebec(g->heap);
    func_0201e794(0xe, g->heap);
    func_0201e75c(0x16, g->heap);
    func_0201e794(9, g->heap);
    func_0201e75c(0x11, g->heap);
    func_02036854(data_ov001_0231e6cc);
    func_0201e7cc(0x14, data_02250458);
}
