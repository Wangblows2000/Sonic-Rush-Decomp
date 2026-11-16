/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 WirelessMgr_GetStatus();
s32 Wireless_OnError();
s32 func_020514ec();
s32 func_ov000_0235fb8c();
void func_ov000_02361378();

void func_ov000_023613c4(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = WirelessMgr_GetStatus(*(u32 *)gTaskCurrent);
    if (t1 == 7) {
        Wireless_OnError();
        return;
    }
    s32 t2 = func_020514ec();
    if (t2 != 0) {
        func_ov000_0235fb8c(t0);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov000_02361378;
    }
}

#pragma thumb reset
