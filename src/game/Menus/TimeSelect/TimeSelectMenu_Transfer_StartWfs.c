/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 WirelessMgr_GetStatus();
s32 Wireless_OnError();
s32 WirelessSession_IsStopped();
s32 WirelessSession_StartWfs();
void TimeSelectMenu_Transfer_WaitWfs();
extern u8 gTaskCurrent[];

void TimeSelectMenu_Transfer_StartWfs(void) {
    s32 t0 = WirelessMgr_GetStatus();
    if (t0 == 7) {
        Wireless_OnError();
        return;
    }
    s32 t1 = WirelessSession_IsStopped();
    if (t1 != 0) {
        WirelessSession_StartWfs();
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)TimeSelectMenu_Transfer_WaitWfs;
    }
}

#pragma thumb reset
