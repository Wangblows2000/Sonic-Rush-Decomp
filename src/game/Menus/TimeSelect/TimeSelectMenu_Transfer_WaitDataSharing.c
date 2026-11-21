/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 WirelessMgr_GetStatus();
s32 Wireless_OnError();
s32 WirelessSession_PollConnected();
s32 Task_Destroy();

void TimeSelectMenu_Transfer_WaitDataSharing(void) {
    s32 t0 = *(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = WirelessMgr_GetStatus(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    if (t1 == 7) {
        Wireless_OnError();
        return;
    }
    s32 t2 = WirelessSession_PollConnected();
    if (t2 != 0) {
        *(u32 *)((u8 *)t0 + 0x20) = 1;
        Task_Destroy(*(u32 *)gTaskCurrent);
    }
}

#pragma thumb reset
