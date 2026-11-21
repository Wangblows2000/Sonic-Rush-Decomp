#include <nitro/types.h>

typedef struct {
    u32 flags;
    void (*cb)(void);
} UnkWork_0204bc90;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_0204bc90 *work;
} UnkTask_0204bc90;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[4];
    u32 flags;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0x24];
    u32 flags;
} UnkStruct_022c4590;

extern UnkTask_0204bc90 *gTaskCurrent;
extern UnkStruct_022c4560 gGameState;
extern UnkStruct_022c4590 data_022c4590;
void Stage_DestroyTasks(void);
void Stage_SaveEventState(void);
void Stage_SaveRestartCamera(void);
void Stage_FreeEventStateBuffers(void);
void Stage_ReleaseZoneResources(void);
void Stage_ReleaseCommonResources(void);
void Stage_FreeZoneFiles(void);
void Stage_FreeCommonFiles(void);
void Vram_ResetTexAndPltt(void);
void VramPltt_Reset(void);
void Sound_ResetHeap(void);
void VRAMTransfer_Clear(void);
void ScreenTransfer_Clear(void);
void PaletteTransfer_Clear(void);
void Task_Destroy(UnkTask_0204bc90 *t);
void Touch_Disable(void);

void StageExit_Main(void) {
    UnkWork_0204bc90 *w = gTaskCurrent->work;
    void (*cb)(void) = w->cb;
    u32 flags = w->flags;
    Stage_DestroyTasks();
    if (gGameState.unk10 == 0) {
        if (gGameState.flags & 0x40) {
            Stage_SaveEventState();
            Stage_SaveRestartCamera();
        } else {
            Stage_FreeEventStateBuffers();
        }
    }
    Stage_ReleaseZoneResources();
    Stage_ReleaseCommonResources();
    if (flags & 2)
        Stage_FreeZoneFiles();
    if (flags & 1)
        Stage_FreeCommonFiles();
    Vram_ResetTexAndPltt();
    VramPltt_Reset();
    Sound_ResetHeap();
    VRAMTransfer_Clear();
    ScreenTransfer_Clear();
    PaletteTransfer_Clear();
    Task_Destroy(gTaskCurrent);
    if (!(gGameState.flags & 0x40))
        data_022c4590.flags &= ~2;
    Touch_Disable();
    if (cb != NULL)
        cb();
}
