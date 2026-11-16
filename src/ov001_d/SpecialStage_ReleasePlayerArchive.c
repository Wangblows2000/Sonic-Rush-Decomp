#include <nitro/types.h>

typedef struct UnkStruct_02313b58 {
    u8 *res;
    u8 obj[4];
} UnkStruct_02313b58;

typedef struct UnkStruct_02313b58_arc {
    u8 pad[0x5c];
} UnkStruct_02313b58_arc;

extern void *gMainHeap;
extern UnkStruct_02313b58 *gSpecialStagePlayerResources;
extern char data_ov001_0231eef0[];

void Model_Release(void *o);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
BOOL func_02021310(void *arc, const char *name, void *narc);
u32 func_02021264(void *arc, u32 idx);
void func_0202566c(u32 p);
BOOL func_020212e0(void *arc);

void SpecialStage_ReleasePlayerArchive(void) {
    vu32 fill1;
    vu32 fill2;
    UnkStruct_02313b58_arc arc;
    u8 *p;
    u32 size;

    if (gSpecialStagePlayerResources->res != NULL) {
        Model_Release(gSpecialStagePlayerResources->obj);
        func_02021310(&arc, data_ov001_0231eef0, gSpecialStagePlayerResources->res);
        func_0202566c(func_02021264(&arc, 0));
        func_020212e0(&arc);
        p = gSpecialStagePlayerResources->res;
        if ((u8 *)OS_GetArenaLo(0) != p) {
            p -= 0x20;
            size = *(u32 *)p;
            if (*(u32 *)(p + size - 4) == 0x02467531) {
                fill1 = 0xcccccccc;
                MIi_CpuClear32(fill1, p, size);
                DC_StoreRange(p, size);
            }
        }
        Heap_Free(gMainHeap, p);
        gSpecialStagePlayerResources->res = NULL;
    }
    p = (u8 *)gSpecialStagePlayerResources;
    if ((u8 *)OS_GetArenaLo(0) != p) {
        p -= 0x20;
        size = *(u32 *)p;
        if (*(u32 *)(p + size - 4) == 0x02467531) {
            fill2 = 0xcccccccc;
            MIi_CpuClear32(fill2, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gMainHeap, p);
    gSpecialStagePlayerResources = NULL;
}
