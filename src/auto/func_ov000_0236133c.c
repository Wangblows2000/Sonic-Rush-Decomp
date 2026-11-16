/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 WirelessMgr_GetStatus();
s32 Wireless_OnError();
s32 func_02051660();
s32 WirelessSession_StartDataSharing();
void func_ov000_02361300();
extern u8 gTaskCurrent[];

void func_ov000_0236133c(void) {
    s32 t0 = WirelessMgr_GetStatus();
    if (t0 == 7) {
        Wireless_OnError();
        return;
    }
    s32 t1 = func_02051660();
    if (t1 != 0) {
        WirelessSession_StartDataSharing(0x40);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov000_02361300;
    }
}

#pragma thumb reset
