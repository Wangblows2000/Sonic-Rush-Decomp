#include <game/Menus/sprobj.h>

BOOL MsgBox_IsClosed(void);
BOOL MsgBox_GetResult(void);
void ResultsScreen_SetLocalQuit(UnkStruct_SprObj *w);
void ResultsScreen_ClearLocalDecided(UnkStruct_SprObj *w);
void ResultsScreen_State_FadeOutExit(void);
void ResultsScreen_Main_CloseWindow(void);

#pragma thumb on

void ResultsScreen_State_QuitPrompt(void) {
    UnkStruct_SprObj *w = gTaskCurrent->work;

    ResultsScreen_DrawHeader(w);
    ResultsScreen_DrawSprite2(w);
    ResultsScreen_DrawSprite1Pair(w);
    ResultsScreen_DrawModels(w);
    ResultsScreen_DrawWinLoseSprites(w, w->unk20, 0x1000);
    w->unk0++;
    if (MsgBox_IsClosed()) {
        w->unk0 = 0;
        if (!MsgBox_GetResult()) {
            w->unkC = 1;
            ResultsScreen_SetLocalQuit(w);
            gTaskCurrent->unk8 = ResultsScreen_State_FadeOutExit;
        } else {
            ResultsScreen_ClearLocalDecided(w);
            ResultsScreen_EnableLineScrollSplit(w);
            gTaskCurrent->unk8 = ResultsScreen_Main_CloseWindow;
        }
    }
}

#pragma thumb reset
