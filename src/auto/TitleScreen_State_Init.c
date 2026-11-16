/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 TitleScreen_InitDisplay();
s32 TitleScreen_LoadAssets();
s32 func_ov001_022d4e4c();
s32 func_ov001_022d4d94();
s32 Touch_Enable();
extern u8 data_0208926c[];
s32 TitleScreen_CreateFadeTask();
s32 func_ov001_022d46f4();
s32 func_ov001_022d44d4();
void TitleScreen_State_WaitForStart();

void TitleScreen_State_Init(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    *(u32 *)t0 = (*(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x1);
    s32 t1 = *(u32 *)t0;
    if (t1 >= 2) {
        TitleScreen_InitDisplay(t1);
        TitleScreen_LoadAssets(t0);
        func_ov001_022d4e4c(t0);
        func_ov001_022d4d94(t0);
        Touch_Enable();
        TitleScreen_CreateFadeTask(*(s16 *)((u8 *)data_0208926c + 8), 0, 8, 0);
        func_ov001_022d46f4((t0 + 0xdc), 1, 0x11, 4, 0x1e0);
        func_ov001_022d46f4((t0 + 0x11c), 1, 0x11, 4, 0x1e0);
        func_ov001_022d44d4(t0);
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)TitleScreen_State_WaitForStart;
    }
}

#pragma thumb reset
