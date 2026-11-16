#include <nitro/types.h>

typedef struct UnkStruct_0230db5c {
    u8 pad0[0x3c];
    u8 objs[7][0x138];
    u8 obj8C4[0x9fc - 0x8c4];
    void *res[4];
    void *unkA0C;
    u8 padA10[0xa40 - 0xa10];
    void *unkA40;
    void *unkA44;
} UnkStruct_0230db5c;

extern UnkStruct_0230db5c *gSpecialStageMap;
extern void *gMainHeap;

void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void Model_Release(void *o);
void NNS_G3dResDefaultRelease(void *res);

static inline void Free_0230db5c(void *p) {
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

void SpecialStage_ReleaseMapModels(void) {
    s32 i;

    Free_0230db5c(gSpecialStageMap->unkA44);
    Free_0230db5c(gSpecialStageMap->unkA40);
    gSpecialStageMap->unkA44 = 0;
    gSpecialStageMap->unkA40 = 0;
    for (i = 0; i < 7; i++)
        Model_Release(gSpecialStageMap->objs[i]);
    Model_Release(gSpecialStageMap->obj8C4);
    for (i = 0; i < 4; i++) {
        NNS_G3dResDefaultRelease(gSpecialStageMap->res[i]);
        Free_0230db5c(gSpecialStageMap->res[i]);
        gSpecialStageMap->res[i] = 0;
    }
    Free_0230db5c(gSpecialStageMap->unkA0C);
    gSpecialStageMap->unkA0C = 0;
    Free_0230db5c(gSpecialStageMap);
    gSpecialStageMap = 0;
}
