/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 VsConnect_Draw();
s32 WirelessMgr_GetStatus();
s32 Task_Destroy();
s32 Wireless_OnError();
s32 WirelessSession_PollConnected();
s32 WirelessSession_CachePartnerName();
extern u8 data_022c4500[];
s32 WirelessMgr_GetJoinInfo_0204319c();
s32 WirelessMgr_StopDataSharing();
void VsConnect_Main_FadeOut();

void VsConnect_Main_SyncJoinInfo(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    VsConnect_Draw(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    s32 t1 = WirelessMgr_GetStatus();
    if (t1 == 7) {
        Task_Destroy(*(u32 *)gTaskCurrent);
        Wireless_OnError();
        return;
    }
    s32 t2 = WirelessSession_PollConnected();
    if (t2 != 0) {
        WirelessSession_CachePartnerName();
        WirelessMgr_GetJoinInfo_0204319c(data_022c4500);
        WirelessMgr_StopDataSharing(1);
        *(u32 *)((u8 *)t0 + 0xc) = 0;
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)VsConnect_Main_FadeOut;
    }
}

#pragma thumb reset
