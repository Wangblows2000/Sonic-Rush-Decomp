/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 WirelessMgr_GetStatus();
s32 WirelessMgr_GetErrorCode();
s32 func_ov001_022c87f8();
extern u8 data_ov001_0231f044[];
void func_ov001_022c8738();
extern u8 gTaskCurrent[];

void func_ov001_022c8774(void) {
    s32 t0 = WirelessMgr_GetStatus();
    if (t0 == 7) {
        WirelessMgr_GetErrorCode();
        func_ov001_022c87f8();
        *(u32 *)data_ov001_0231f044 = 1;
        return;
    }
    if (t0 == 6) {
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov001_022c8738;
    }
}

#pragma thumb reset
