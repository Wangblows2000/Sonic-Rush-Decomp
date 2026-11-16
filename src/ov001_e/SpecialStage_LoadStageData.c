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
extern UnkStruct_02314508 *gSpecialStageData;
extern s32 gSpecialStageFileTable[];

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);
s32 func_ov001_02314204(s32 a);
void func_ov001_023141cc(s32 a);

static inline u8 *Alloc(u32 sz) {
    u32 size = sz;
    u8 *p = Heap_Alloc(gMainHeap, size, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x02467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

void SpecialStage_LoadStageData(s32 idx) {
    UnkStruct_02314508 *g;

    gSpecialStageData = (UnkStruct_02314508 *)Alloc(0x40);
    MI_CpuFill8(gSpecialStageData, 0, sizeof(UnkStruct_02314508));
    g = gSpecialStageData;
    g->idx = idx;
    g->unk8 = 4;
    g->unk14 = func_ov001_02314204(gSpecialStageFileTable[g->idx]);
    func_ov001_023141cc(g->unk14);
}
