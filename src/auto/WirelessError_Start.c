/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 WirelessError_InitDisplay();
s32 Touch_Enable();
s32 SysSound_Init();
s32 SysSound_PlaySE();
s32 MsgBox_Create();
void WirelessError_Main_WaitClose();
s32 Task_Create();

void WirelessError_Start(void) {
    WirelessError_InitDisplay();
    Touch_Enable();
    SysSound_Init();
    SysSound_PlaySE(6);
    MsgBox_Create(0xb, 1, 2, 0, 0, 0, 0, 0x200010);
    Task_Create((u32)WirelessError_Main_WaitClose, 0, 0, 0, 0, 4);
}

#pragma thumb reset
