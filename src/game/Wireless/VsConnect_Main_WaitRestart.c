/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 VsConnect_Draw();
s32 WirelessMgr_GetStatus();
s32 Task_Destroy();
s32 Wireless_OnError();
s32 WirelessSession_IsStopped();
s32 WirelessSession_StartDataSharing();
void VsConnect_Main_SyncHostInfo();

void VsConnect_Main_WaitRestart(void) {
    VsConnect_Draw(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    s32 t0 = WirelessMgr_GetStatus();
    if (t0 == 7) {
        Task_Destroy(*(u32 *)gTaskCurrent);
        Wireless_OnError();
        return;
    }
    s32 t1 = WirelessSession_IsStopped();
    if (t1 != 0) {
        WirelessSession_StartDataSharing(0x40);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)VsConnect_Main_SyncHostInfo;
    }
}

#pragma thumb reset
