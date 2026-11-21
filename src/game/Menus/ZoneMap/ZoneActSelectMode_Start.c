#include <nitro/types.h>

extern s32 gGameState;
extern s32 gSaveDataBusy;
extern u32 gSystemFlags;

void StoryProgress_RestoreFromSave(void);
void ZoneActSelectMode_InitDisplay(void);
void SysSound_Release(void);
void SysSound_Init(void);
void SysSound_PlayBGM(s32 a);
void NaviBg_Load(s32 a, s32 b);
void NaviM_Load(void);
void NaviMsg_Create(void);
void NaviM_ResetPending(void);
void NaviM_Create(void);
void NaviMsg_SetVisible(s32 a);
void ZoneActSelect_Create(s32 a, s32 b, s32 c);
void Touch_Enable(void);
void *Task_Create(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void ZoneActSelectMode_Main(void);

#pragma thumb on
void ZoneActSelectMode_Start(void) {
    gGameState = 0;
    StoryProgress_RestoreFromSave();
    ZoneActSelectMode_InitDisplay();
    SysSound_Release();
    SysSound_Init();
    SysSound_PlayBGM(4);
    NaviBg_Load(0, 0);
    NaviM_Load();
    NaviMsg_Create();
    NaviM_ResetPending();
    NaviM_Create();
    NaviMsg_SetVisible(0);
    ZoneActSelect_Create(0, 0, 3);
    Touch_Enable();
    gSaveDataBusy = 0;
    gSystemFlags &= ~0x10;
    Task_Create(ZoneActSelectMode_Main, 0, 0, 0x5000, 0x2000, 0);
}
#pragma thumb reset
