#include <nitro/types.h>

extern void *gMainHeap;
extern void *data_ov000_0236aa8c;
extern u8 data_ov000_023687e0[];
extern void *data_ov000_0236aa9c[];

void func_02021310(void *buf, void *a, void *b);
void *func_02021264(void *buf, s32 a);
void func_020212e0(void *buf);
void *Heap_Alloc(void *heap, u32 size, s32 align);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
u32 func_ov000_02335d00(void *res);
u32 func_ov000_02335cf4(void *res);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
void *Compress_GetData(void *res);
void Mem_Copy(void *src, void *dst, u32 size);
void func_ov000_02342178(void *res, s32 a, s32 b);

static inline void *UnkAlloc_02335954(u32 size, s32 align) {
    u32 sz = (size + 0x43) & ~0x1f;
    u8 *p = Heap_Alloc(gMainHeap, sz, align);
    if (OS_GetArenaLo(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, sz);
        *(u32 *)p = sz;
        *(u32 *)(p + sz - 4) = 0x2467531;
        DC_StoreRange(p, sz);
        p += 0x20;
    }
    return p;
}

static inline void UnkLoad_02335954(void *arc, s32 idx, void **slot) {
    void *res = func_02021264(arc, idx);
    void *dst;
    *slot = UnkAlloc_02335954(*(u32 *)res >> 8, 0x20);
    dst = *slot;
    switch (func_ov000_02335d00(res)) {
    case 0x10:
        MI_UncompressLZ16(res, dst);
        break;
    case 0x20:
        MI_UncompressHuffman(res, dst);
        break;
    case 0x30:
        MI_UncompressRL16(res, dst);
        break;
    default:
        Mem_Copy(Compress_GetData(res), dst, func_ov000_02335cf4(res));
        break;
    }
    DC_StoreRange(dst, func_ov000_02335cf4(res));
}

void func_ov000_02335954(void) {
    u8 buf[0x60];
    void **tbl = data_ov000_0236aa9c;

    func_02021310(buf, data_ov000_023687e0, data_ov000_0236aa8c);
    UnkLoad_02335954(buf, 1, &tbl[0]);
    UnkLoad_02335954(buf, 2, &tbl[1]);
    UnkLoad_02335954(buf, 0, &tbl[2]);
    func_ov000_02342178(func_02021264(buf, 3), 2, 0xff);
    func_020212e0(buf);
}
