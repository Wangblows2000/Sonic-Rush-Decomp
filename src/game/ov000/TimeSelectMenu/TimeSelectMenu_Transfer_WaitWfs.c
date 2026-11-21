/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 WirelessMgr_GetStatus();
s32 Wireless_OnError();
s32 WirelessSession_PollConnected();
s32 TimeSelectMenu_LoadStageFiles();
void TimeSelectMenu_Transfer_LoadFiles();

void TimeSelectMenu_Transfer_WaitWfs(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = WirelessMgr_GetStatus(*(u32 *)gTaskCurrent);
    if (t1 == 7) {
        Wireless_OnError();
        return;
    }
    s32 t2 = WirelessSession_PollConnected();
    if (t2 != 0) {
        TimeSelectMenu_LoadStageFiles(t0);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)TimeSelectMenu_Transfer_LoadFiles;
    }
}

#pragma thumb reset
