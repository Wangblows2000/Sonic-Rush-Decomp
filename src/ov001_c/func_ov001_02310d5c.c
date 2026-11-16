#include <nitro/types.h>

typedef struct UnkStruct_02310d5c_e {
    u8 pad[0xe4];
} UnkStruct_02310d5c_e;

typedef struct UnkStruct_02310d5c {
    u16 count;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    UnkStruct_02310d5c_e *ents;
    UnkStruct_02310d5c_e **ptrs;
    u8 unk10[0xec - 0x10];
    void *unkEC;
    void *unkF0;
    u8 unkF4[0x1d0 - 0xf4];
    void *unk1D0;
    void *unk1D4;
} UnkStruct_02310d5c;

extern void *gMainHeap;

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void MI_CpuFill8(void *dst, u8 v, u32 size);

static inline u8 *Alloc_02310d5c(u32 size) {
    u32 sz = (size + 0x43) & ~0x1f;
    u8 *blk = Heap_Alloc(gMainHeap, sz, 0x20);
    if ((u8 *)func_02006abc(0) != blk) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, blk, sz);
        *(u32 *)blk = sz;
        *(u32 *)(blk + sz - 4) = 0x02467531;
        DC_StoreRange(blk, sz);
        blk += 0x20;
    }
    return blk;
}

UnkStruct_02310d5c *func_ov001_02310d5c(s32 n) {
    UnkStruct_02310d5c *g;
    s32 i;

    g = (UnkStruct_02310d5c *)Alloc_02310d5c(0x1e4);
    MI_CpuFill8(g, 0, 0x1e4);
    g->ents = (UnkStruct_02310d5c_e *)Alloc_02310d5c(n * 0xe4);
    g->ptrs = (UnkStruct_02310d5c_e **)Alloc_02310d5c(n * 4);
    MI_CpuFill8(g->ents, 0, n * 0xe4);
    MI_CpuFill8(g->ptrs, 0, n * 4);
    g->count = n;
    g->unk2 = 0;
    g->unk4 = 0;
    g->unk6 = 0;
    g->unkEC = g->unkF4;
    g->unkF0 = 0;
    g->unk1D0 = 0;
    g->unk1D4 = g->unk10;
    for (i = 0; i < g->count; i++)
        g->ptrs[i] = &g->ents[i];
    return g;
}
