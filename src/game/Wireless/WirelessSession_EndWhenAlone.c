/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Wireless_EnableErrorCallback();
void WirelessSessionEnd_Main_WaitAlone();
s32 Task_Create();

void WirelessSession_EndWhenAlone(s32 p0) {
    Wireless_EnableErrorCallback(0);
    s32 t0 = Task_Create((u32)WirelessSessionEnd_Main_WaitAlone, 0, 0, 0, 0, 4);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x10);
    *(u32 *)t1 = p0;
}

#pragma thumb reset
