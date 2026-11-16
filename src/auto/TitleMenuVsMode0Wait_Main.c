/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 VsMenu_IsClosedForMode0();
s32 Task_Destroy();
s32 func_ov001_022ddd88();
extern u8 gSaveDataBusy[];
extern u8 gSystemFlags[];
s32 func_ov001_022d731c();
s32 TitleMenu_OpenVsPage();

void TitleMenuVsMode0Wait_Main(void) {
    s32 t0 = *(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = VsMenu_IsClosedForMode0(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    if (t1 != 0) {
        Task_Destroy(*(u32 *)gTaskCurrent);
        s32 t2 = func_ov001_022ddd88();
        if (t2 != 0) {
            *(u32 *)gSaveDataBusy = 0;
            s32 t3 = (*(u32 *)gSystemFlags & ~0x10);
            *(u32 *)gSystemFlags = t3;
            func_ov001_022d731c(t0, gSystemFlags, t3);
            TitleMenu_OpenVsPage(t0, 1, 1);
            return;
        }
        *(u32 *)t0 = 4;
    }
}

#pragma thumb reset
