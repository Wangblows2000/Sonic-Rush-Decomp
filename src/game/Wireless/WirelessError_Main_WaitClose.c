/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 MsgBox_IsClosed();
s32 Touch_Disable();
s32 WirelessError_ResetBgAffine();
extern u8 gTaskCurrent[];
void WirelessError_Main_FadeOut();

void WirelessError_Main_WaitClose(void) {
    s32 t0 = MsgBox_IsClosed();
    if (t0 != 0) {
        Touch_Disable();
        WirelessError_ResetBgAffine();
        *(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) = 0;
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)WirelessError_Main_FadeOut;
    }
}

#pragma thumb reset
