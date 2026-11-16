/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02054b50();
s32 Touch_Enable();
s32 SysSound_Init();
s32 SysSound_PlaySE();
s32 MsgBox_Create();
void func_02054d3c();
s32 Task_Create();

void WirelessError_Start(void) {
    func_02054b50();
    Touch_Enable();
    SysSound_Init();
    SysSound_PlaySE(6);
    MsgBox_Create(0xb, 1, 2, 0, 0, 0, 0, 0x200010);
    Task_Create((u32)func_02054d3c, 0, 0, 0, 0, 4);
}

#pragma thumb reset
