/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 ZoneActSelect_IsFinished();
s32 ZoneActSelect_GetResult();
s32 ZoneActSelectMode_Shutdown();
extern u8 gTaskCurrent[];
s32 Task_Destroy();
s32 ZoneActSelectMode_ExitToTitleMenu();
s32 ZoneActSelectMode_StartStage();

void ZoneActSelectMode_Main(void) {
    s32 t0 = ZoneActSelect_IsFinished();
    if (t0 != 0) {
        s32 t1 = ZoneActSelect_GetResult();
        ZoneActSelectMode_Shutdown();
        Task_Destroy(*(u32 *)gTaskCurrent);
        if (t1 < 0) {
            ZoneActSelectMode_ExitToTitleMenu();
            return;
        }
        ZoneActSelectMode_StartStage(t1);
    }
}

#pragma thumb reset
