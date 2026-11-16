#include <nitro/types.h>

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    void *unkC;
    u16 *unk10;
    void *unk14;
    void *unk18;
} UnkStruct_0236aabc;

typedef struct {
    u8 pad0[0x24];
    void *unk24;
    void *unk28;
    u8 pad2C[0x114 - 0x2C];
    s32 unk114;
    s32 unk118;
    s32 unk11C;
    s32 unk120;
    UnkStruct_0236aabc unk124;
} UnkStruct_0236aad8;

typedef struct {
    u8 pad0[0x24];
    void *unk24;
    void *unk28;
} UnkStruct_0236ab3c;

typedef struct {
    u8 pad0[0xE];
    u16 unkE;
    u16 unk10;
} UnkStruct_0236abd8;

extern void *gMainHeap;
extern UnkStruct_0236aabc data_ov000_0236aabc;
extern UnkStruct_0236aad8 gCamera;
extern UnkStruct_0236ab3c data_ov000_0236ab3c;
extern UnkStruct_0236abd8 data_ov000_0236abd8;

void *Heap_Alloc(void *heap, u32 size, s32 align);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);

static inline void *UnkAlloc_0233a27c(u32 size, s32 align) {
    u8 *p;
    size = (size + 0x43) & ~0x1f;
    p = Heap_Alloc(gMainHeap, size, align);
    if (OS_GetArenaLo(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x2467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

void func_ov000_0233a27c(void) {
    UnkStruct_0236aad8 *a = &gCamera;
    UnkStruct_0236ab3c *c;

    a->unk124.unk0 = data_ov000_0236aabc.unk0;
    a->unk124.unk4 = data_ov000_0236aabc.unk4;
    a->unk124.unk8 = data_ov000_0236aabc.unk8;
    a->unk124.unkC = data_ov000_0236aabc.unkC;
    a->unk124.unk10 = data_ov000_0236aabc.unk10;
    a->unk124.unk14 = data_ov000_0236aabc.unk14;
    a->unk124.unk18 = data_ov000_0236aabc.unk18;
    data_ov000_0236abd8.unkE = a->unk124.unk10[0] << 6;
    data_ov000_0236abd8.unk10 = a->unk124.unk10[1] << 6;
    a->unk114 = 0x800;
    a->unk118 = 0x800;
    a->unk11C = (data_ov000_0236abd8.unkE - 8) << 8;
    a->unk120 = (data_ov000_0236abd8.unk10 - 8) << 8;
    a->unk24 = UnkAlloc_0233a27c(0x2000, 0x20);
    a->unk28 = UnkAlloc_0233a27c(0x2000, 0x20);
    c = &data_ov000_0236ab3c;
    c->unk24 = UnkAlloc_0233a27c(0x2000, 0x20);
    c->unk28 = UnkAlloc_0233a27c(0x2000, 0x20);
}
