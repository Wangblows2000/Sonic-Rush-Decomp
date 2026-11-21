#include <nitro/types.h>

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

typedef struct {
    u8 pad0[0x1c];
    u8 stage;
    u8 act;
} UnkStruct_022c4560;

extern void *gMainHeap;
extern u8 *gZoneActArchive;
extern UnkStruct_022c4560 gGameState;
extern void (*gStageZoneFreeFuncTable[][3])(void);

void Stage_FreeMapInfo(void);
void Stage_FreeZoneFile(void);
void Stage_FreeZoneSoundData(void);
void NaviMsg_CloseFiles(void);
void NaviB_Release(void);

static inline void Free_0204b528(u8 *p) {
    if ((u8 *)OS_GetArenaLo(0) != p) {
        u32 size;
        p -= 0x20;
        size = *(u32 *)p;
        if (*(u32 *)(p + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gMainHeap, p);
}

void Stage_FreeZoneFiles(void) {
    gStageZoneFreeFuncTable[gGameState.stage][gGameState.act]();
    Stage_FreeMapInfo();
    Stage_FreeZoneFile();
    if (gZoneActArchive != NULL) {
        Free_0204b528(gZoneActArchive);
        gZoneActArchive = NULL;
    }
    Stage_FreeZoneSoundData();
    if (gGameState.stage < 9 && gGameState.act < 2)
        return;
    NaviMsg_CloseFiles();
    NaviB_Release();
}
