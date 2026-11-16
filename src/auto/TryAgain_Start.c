/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 TryAgain_Create();
s32 TryAgain_InitDisplay();
s32 TryAgain_LoadAssets();
s32 TryAgain_InitSprites();
s32 TryAgain_InitBackgrounds();
s32 Touch_Enable();
s32 TryAgain_CreateBgScrollTask();
extern u8 data_0208926c[];
s32 TryAgain_CreateFadeTask();
s32 func_ov001_022d37ac();
s32 SysSound_Init();
s32 SysSound_PlayBGM();

void TryAgain_Start(void) {
    s32 t0 = TryAgain_Create();
    TryAgain_InitDisplay();
    TryAgain_LoadAssets(t0);
    TryAgain_InitSprites(t0);
    TryAgain_InitBackgrounds(t0);
    Touch_Enable();
    TryAgain_CreateBgScrollTask();
    TryAgain_CreateFadeTask(*(s16 *)((u8 *)data_0208926c + 8), 0, 0x10, 2);
    func_ov001_022d37ac(t0);
    SysSound_Init();
    SysSound_PlayBGM(0);
}

#pragma thumb reset
