/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_02063030();
s32 WirelessMgr_GetStatus();
s32 Task_Destroy();
s32 Wireless_OnError();
s32 func_020514ec();
s32 StageLoader_Init();
s32 StageLoader_LoadCommonFiles();
void func_02063588();

void func_020635d0(void) {
    func_02063030(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    s32 t0 = WirelessMgr_GetStatus();
    if (t0 == 7) {
        Task_Destroy(*(u32 *)gTaskCurrent);
        Wireless_OnError();
        return;
    }
    s32 t1 = func_020514ec();
    if (t1 != 0) {
        StageLoader_Init(1);
        StageLoader_LoadCommonFiles();
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_02063588;
    }
}

#pragma thumb reset
