#include <nitro/types.h>

typedef struct UnkStruct_0230f8b4 {
    void *unk0;
    u8 objs[9][0xf4];
    void *res[5];
    u8 anims[5][0x138];
    void *unkEC4;
} UnkStruct_0230f8b4;

extern UnkStruct_0230f8b4 *gSpecialStageGimmickModels;
extern void *gMainHeap;

void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void Model_Release(void *o);
void NNS_G3dResDefaultRelease(void *res);

static inline void Free_0230f8b4(void *p) {
    u32 size;
    if (OS_GetArenaLo(0) != p) {
        p = (u8 *)p - 0x20;
        size = *(u32 *)p;
        if (*(u32 *)((u8 *)p + size - 4) == 0x2467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gMainHeap, p);
}

void SpecialStage_ReleaseGimmickModels(void) {
    UnkStruct_0230f8b4 *g = gSpecialStageGimmickModels;
    s32 i;
    s32 j;

    for (i = 0; i < 9; i++)
        Model_Release(g->objs[i]);
    for (i = 0; i < 5; i++) {
        NNS_G3dResDefaultRelease(g->res[i]);
        Model_Release(g->anims[i]);
        Free_0230f8b4(g->res[i]);
    }
    if (g->unk0 != NULL) {
        Free_0230f8b4(g->unk0);
        g->unk0 = NULL;
    }
    Free_0230f8b4(g->unkEC4);
    g->unkEC4 = NULL;
    Free_0230f8b4(gSpecialStageGimmickModels);
    gSpecialStageGimmickModels = NULL;
}
