/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_02063030();
s32 StageLoader_LoadCommonFiles();
s32 Task_Destroy();
s32 Wireless_OnError();
s32 WirelessSession_Stop();
void func_0206353c();

void func_02063588(void) {
    func_02063030(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    s32 t0 = StageLoader_LoadCommonFiles();
    if (t0 == 2) {
        Task_Destroy(*(u32 *)gTaskCurrent);
        Wireless_OnError();
        return;
    }
    if (t0 == 1) {
        WirelessSession_Stop();
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_0206353c;
    }
}

#pragma thumb reset
