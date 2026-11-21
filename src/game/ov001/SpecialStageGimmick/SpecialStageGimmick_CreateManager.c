#include <nitro/types.h>

typedef struct UnkStruct_0230f804_n {
    u8 pad0[0x1dc];
    void *unk1DC;
} UnkStruct_0230f804_n;

typedef struct UnkStruct_0230f804 {
    UnkStruct_0230f804_n *node;
} UnkStruct_0230f804;

extern void *gMainHeap;

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);
UnkStruct_0230f804_n *SpecialStageObjList_Create(s32 a);
void SpecialStageGimmick_Unk_ov001_0230f0f4(void);

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

UnkStruct_0230f804 *SpecialStageGimmick_CreateManager(s32 a) {
    UnkStruct_0230f804 *w = (UnkStruct_0230f804 *)Alloc(0x40);

    MI_CpuFill8(w, 0, sizeof(UnkStruct_0230f804));
    w->node = SpecialStageObjList_Create(a);
    w->node->unk1DC = SpecialStageGimmick_Unk_ov001_0230f0f4;
    return w;
}
