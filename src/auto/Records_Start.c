/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Records_Create();
s32 Records_InitDisplay();
s32 Records_LoadAssets();
s32 Records_Unk_ov001_022edb3c();
s32 Records_Unk_ov001_022eda54();
s32 Records_Unk_ov001_022ecae4();
s32 Touch_Enable();
extern u8 data_0208926c[];
s32 Records_CreateFadeTask();
void Records_ScrollBg_Main();
s32 Task_Create();
s32 SysSound_Init();
s32 SysSound_PlayBGM();

void Records_Start(void) {
    s32 t0 = Records_Create();
    Records_InitDisplay();
    Records_LoadAssets(t0);
    Records_Unk_ov001_022edb3c(t0);
    Records_Unk_ov001_022eda54(t0);
    Records_Unk_ov001_022ecae4(t0);
    Touch_Enable();
    Records_CreateFadeTask(*(s16 *)((u8 *)data_0208926c + 8), 0, 0x10, 0);
    Task_Create((u32)Records_ScrollBg_Main, 0, 0, 0x2000, 0x1000, 0);
    SysSound_Init();
    SysSound_PlayBGM(0);
}

#pragma thumb reset
