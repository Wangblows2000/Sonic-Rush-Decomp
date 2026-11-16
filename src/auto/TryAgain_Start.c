/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 TryAgain_Create();
s32 func_ov001_022d3110();
s32 TryAgain_LoadAssets();
s32 func_ov001_022d3fc8();
s32 func_ov001_022d3f10();
s32 Touch_Enable();
s32 TryAgain_CreateBgScrollTask();
extern u8 data_0208926c[];
s32 TryAgain_CreateFadeTask();
s32 func_ov001_022d37ac();
s32 SysSound_Init();
s32 func_02052168();

void TryAgain_Start(void) {
    s32 t0 = TryAgain_Create();
    func_ov001_022d3110();
    TryAgain_LoadAssets(t0);
    func_ov001_022d3fc8(t0);
    func_ov001_022d3f10(t0);
    Touch_Enable();
    TryAgain_CreateBgScrollTask();
    TryAgain_CreateFadeTask(*(s16 *)((u8 *)data_0208926c + 8), 0, 0x10, 2);
    func_ov001_022d37ac(t0);
    SysSound_Init();
    func_02052168(0);
}

#pragma thumb reset
