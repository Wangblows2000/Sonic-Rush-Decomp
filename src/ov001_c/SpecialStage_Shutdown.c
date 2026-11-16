#include <nitro/types.h>

typedef struct UnkStruct_0230c104_br {
    u8 pad0[0x20];
    u16 b0 : 1;
    u16 p1 : 5;
    u16 b6 : 2;
    u16 p8 : 1;
    u16 b9 : 1;
    u16 b10 : 1;
    u16 p11 : 1;
    u16 b12 : 1;
    u16 p13 : 3;
    u16 e0 : 5;
    u16 p5 : 3;
    u16 e8 : 5;
    u16 p13b : 3;
} UnkStruct_0230c104_br;

typedef struct UnkStruct_0230c104_p {
    s16 x;
    s16 y;
} UnkStruct_0230c104_p;

typedef struct UnkStruct_0230c104 {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[4];
    void *taskC;
    void *task10;
    void *task14;
    u8 *unk18;
    u8 *unk1C;
    u8 pad20[4];
    s32 unk24;
} UnkStruct_0230c104;

typedef struct UnkStruct_0230c104_4560 {
    u8 pad0[0x28];
    u16 unk28;
} UnkStruct_0230c104_4560;

extern UnkStruct_0230c104 *gSpecialStageWork;
extern void *data_ov001_0231f5c4;
extern void *gMainHeap;
extern UnkStruct_0230c104_4560 gGameState;
extern UnkStruct_0230c104_br gDisplayMain;
extern UnkStruct_0230c104_p gDisplaySub[];

void Task_Destroy(void *t);
void SpecialStageGimmick_DestroyManager(u8 *blk);
void SpecialStagePlayer_Destroy(u8 *blk);
void SpecialStage_ShutdownDummy(void);
void SpecialStage_ReleaseActArchive(void);
void SpecialStage_ReleaseGimmickModels(void);
void SpecialStage_ReleaseMapModels(void);
void SpecialStage_ReleasePlayerArchive(void);
void SpecialStageCamera_Shutdown(void);
void SpecialStage_ReleaseStageData(void);
void SpecialStageHud_Destroy(void);
void SpecialStageTrick_Shutdown(void);
void SpecialStage_ReleaseHudAssets(void *p);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void G3X_SetFog(BOOL enable, u32 fogMode, u32 fogSlope, s32 fogOffset);

static inline void Free_0230c104(void *p) {
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

void SpecialStage_Shutdown(void) {
    UnkStruct_0230c104 *g = gSpecialStageWork;
    s32 i;

    if (g->taskC != NULL)
        Task_Destroy(g->taskC);
    if (g->task10 != NULL)
        Task_Destroy(g->task10);
    if (g->task14 != NULL)
        Task_Destroy(g->task14);
    SpecialStageGimmick_DestroyManager(g->unk1C);
    if (g->unk18 != NULL) {
        SpecialStagePlayer_Destroy(g->unk18);
        g->unk18 = NULL;
    }
    SpecialStage_ShutdownDummy();
    SpecialStage_ReleaseActArchive();
    SpecialStage_ReleaseGimmickModels();
    SpecialStage_ReleaseMapModels();
    SpecialStage_ReleasePlayerArchive();
    SpecialStageCamera_Shutdown();
    SpecialStage_ReleaseStageData();
    SpecialStageHud_Destroy();
    SpecialStageTrick_Shutdown();
    SpecialStage_ReleaseHudAssets(data_ov001_0231f5c4);
    Free_0230c104(data_ov001_0231f5c4);
    data_ov001_0231f5c4 = NULL;
    if (g->unk24 != 0)
        gGameState.unk28 |= 1 << g->unk4;
    Free_0230c104(gSpecialStageWork);
    gSpecialStageWork = NULL;
    gDisplayMain.b0 = 0;
    gDisplayMain.b9 = 0;
    gDisplayMain.e0 = 0;
    gDisplayMain.b10 = 0;
    gDisplayMain.b12 = 0;
    gDisplayMain.b6 = 0;
    gDisplayMain.e8 = 0;
    G3X_SetFog(0, 0, 4, 0);
    for (i = 0; i < 4; i++) {
        ((UnkStruct_0230c104_p *)&gDisplayMain)[i].x = 0;
        ((UnkStruct_0230c104_p *)&gDisplayMain)[i].y = 0;
        gDisplaySub[i].x = 0;
        gDisplaySub[i].y = 0;
    }
}
