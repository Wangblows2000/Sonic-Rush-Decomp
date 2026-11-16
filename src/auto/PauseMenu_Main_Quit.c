/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 BrightnessFade_IsDone();
s32 Task_Destroy();
extern u8 gGameState[];
void PauseMenu_QuitToTryAgain();
s32 StageExit_Create();
s32 ExtraBoss_Shutdown();
s32 Task_DestroyByTag();
s32 SceneChange_ToPlayerSelectMenuMode0();
s32 SpecialStage_RequestExit();
void PauseMenu_QuitToMap();
s32 func_02053bec();
s32 PauseMenu_Draw();

void PauseMenu_Main_Quit(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = *(u16 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x4);
    s32 t2 = BrightnessFade_IsDone(*(u32 *)gTaskCurrent);
    if (t2 != 0) {
        Task_Destroy(*(u32 *)gTaskCurrent);
        if (*(u32 *)((u8 *)gGameState + 0x10) == 2) {
            StageExit_Create((u32)PauseMenu_QuitToTryAgain, 3);
            return;
        }
        s32 t3 = *(u8 *)((u8 *)gGameState + 0x1c);
        if (t3 == 8) {
            s32 t4 = *(u8 *)((u8 *)gGameState + 0x1d);
            if (t4 == 2) {
                ExtraBoss_Shutdown(t4, t3);
                Task_DestroyByTag(1);
                Task_DestroyByTag(2);
                Task_DestroyByTag(3);
                Task_DestroyByTag(4);
                SceneChange_ToPlayerSelectMenuMode0();
                return;
            }
        }
        if ((t1 & 0x20) != 0) {
            SpecialStage_RequestExit(0);
            return;
        }
        StageExit_Create((u32)PauseMenu_QuitToMap, 3);
        s32 t5 = *(u8 *)((u8 *)gGameState + 0x1c);
        if ((u32)t5 < (u32)9) {
            if ((u32)*(u8 *)((u8 *)gGameState + 0x1d) < (u32)2) {
                return;
            }
        }
        func_02053bec(1, t5);
        return;
    }
    PauseMenu_Draw(t0);
}
