/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 WirelessMgr_GetStatus();
s32 WirelessMgr_GetErrorCode();
s32 OptionsWfsHost_Destroy();
extern u8 gOptionsWfsHostError[];
void OptionsWfsHost_State_InitCommonFiles();
extern u8 gTaskCurrent[];

void OptionsWfsHost_State_WaitWfsReady(void) {
    s32 t0 = WirelessMgr_GetStatus();
    if (t0 == 7) {
        WirelessMgr_GetErrorCode();
        OptionsWfsHost_Destroy();
        *(u32 *)gOptionsWfsHostError = 1;
        return;
    }
    if (t0 == 6) {
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)OptionsWfsHost_State_InitCommonFiles;
    }
}

#pragma thumb reset
