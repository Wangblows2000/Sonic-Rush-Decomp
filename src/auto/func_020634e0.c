/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_02063030();
s32 WirelessMgr_GetStatus();
s32 Task_Destroy();
s32 Wireless_OnError();
s32 func_020514ec();
extern u8 data_022c44f8[];
s32 WirelessMgr_GetHostInfo_02043204();
s32 WirelessMgr_StopDataSharing();
void func_0206340c();

void func_020634e0(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    func_02063030(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    s32 t1 = WirelessMgr_GetStatus();
    if (t1 == 7) {
        Task_Destroy(*(u32 *)gTaskCurrent);
        Wireless_OnError();
        return;
    }
    s32 t2 = func_020514ec();
    if (t2 != 0) {
        WirelessMgr_GetHostInfo_02043204(data_022c44f8);
        WirelessMgr_StopDataSharing(1);
        *(u32 *)((u8 *)t0 + 0xc) = 0;
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_0206340c;
    }
}

#pragma thumb reset
