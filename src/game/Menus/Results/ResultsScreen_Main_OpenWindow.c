/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 ResultsScreen_DrawHeader();
s32 ResultsScreen_DrawSprite2();
s32 ResultsScreen_DrawSprite1Pair();
s32 ResultsScreen_DrawModels();
s32 ResultsScreen_DrawWinLoseSprites();
s32 ResultsScreen_EaseValue();
s32 ResultsScreen_SetWindowLineScroll();
s32 ResultsScreen_DrawSprite16();
s32 ResultsScreen_DisableLineScrollHidden();
s32 MsgBox_Create();
void ResultsScreen_State_QuitPrompt();
void ResultsScreen_State_FadeOutExit();
void ResultsScreen_State_WaitPartner();

void ResultsScreen_Main_OpenWindow(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    ResultsScreen_DrawHeader(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    ResultsScreen_DrawSprite2(t0);
    ResultsScreen_DrawSprite1Pair(t0);
    ResultsScreen_DrawModels(t0);
    ResultsScreen_DrawWinLoseSprites(t0, *(u32 *)((u8 *)t0 + 0x20), 0x1000);
    s32 t1 = ResultsScreen_EaseValue(0, 0x44, 0xa, *(u32 *)t0, 0x2000);
    ResultsScreen_SetWindowLineScroll(t0, t1);
    if (*(u32 *)((u8 *)t0 + 0x10) == 0) {
        if (*(u32 *)((u8 *)t0 + 0x8) == 2) {
            ResultsScreen_DrawSprite16(t0);
        }
    }
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    if ((u32)*(u32 *)t0 >= (u32)0xa) {
        *(u32 *)t0 = 0;
        ResultsScreen_DisableLineScrollHidden(t0);
        if (*(u32 *)((u8 *)t0 + 0x10) != 0) {
            if (*(u32 *)((u8 *)t0 + 0x8) == 2) {
                MsgBox_Create(9, 3, 1, 1, 0, 0, 0xe, 0x200010);
                *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)ResultsScreen_State_QuitPrompt;
                return;
            }
            *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)ResultsScreen_State_FadeOutExit;
            return;
        }
        if (*(u32 *)((u8 *)t0 + 0x8) == 2) {
            *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)ResultsScreen_State_WaitPartner;
            return;
        }
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)ResultsScreen_State_FadeOutExit;
    }
}

#pragma thumb reset
