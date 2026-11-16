#include <nitro/types.h>

typedef struct UnkStruct_02314508 {
    s32 idx;
    u8 pad4[4];
    s32 unk8;
    u8 padC[0x14 - 0xc];
    s32 unk14;
    u8 pad18[4];
} UnkStruct_02314508;

extern void *gMainHeap;
extern UnkStruct_02314508 *data_ov001_0231f69c;
extern s32 data_ov001_0231ef64[];

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);
s32 func_ov001_02314204(s32 a);
void func_ov001_023141cc(s32 a);

static inline u8 *Alloc(u32 sz) {
    u32 size = sz;
    u8 *p = Heap_Alloc(gMainHeap, size, 0x20);
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

void func_ov001_02314508(s32 idx) {
    UnkStruct_02314508 *g;

    data_ov001_0231f69c = (UnkStruct_02314508 *)Alloc(0x40);
    MI_CpuFill8(data_ov001_0231f69c, 0, sizeof(UnkStruct_02314508));
    g = data_ov001_0231f69c;
    g->idx = idx;
    g->unk8 = 4;
    g->unk14 = func_ov001_02314204(data_ov001_0231ef64[g->idx]);
    func_ov001_023141cc(g->unk14);
}
