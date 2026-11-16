/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 ResultsScreen_DrawHeader();
s32 ResultsScreen_DrawSprite2();
s32 ResultsScreen_DrawSprite1Pair();
s32 ResultsScreen_DrawModels();
s32 ResultsScreen_DrawWinLoseSprites();
s32 ResultsScreen_DrawSprite16();
s32 ResultsScreen_HasPartnerDecided();
s32 ResultsScreen_HasPartnerQuit();
s32 ResultsScreen_EnableLineScrollSplit();
void ResultsScreen_Main_CloseWindow();
void ResultsScreen_State_FadeOutExit();

void ResultsScreen_State_WaitPartner(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    ResultsScreen_DrawHeader(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    ResultsScreen_DrawSprite2(t0);
    ResultsScreen_DrawSprite1Pair(t0);
    ResultsScreen_DrawModels(t0);
    ResultsScreen_DrawWinLoseSprites(t0, *(u32 *)((u8 *)t0 + 0x20), 0x1000);
    ResultsScreen_DrawSprite16(t0);
    s32 t1 = ResultsScreen_HasPartnerDecided(t0);
    s32 t2 = ResultsScreen_HasPartnerQuit(t0);
    s32 t3 = (*(u32 *)t0 + 0x1);
    *(u32 *)t0 = t3;
    if (t1 == 0) {
        *(u32 *)t0 = 0;
        ResultsScreen_EnableLineScrollSplit(t0, t3);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)ResultsScreen_Main_CloseWindow;
        return;
    }
    if (t2 != 0) {
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)t0 + 0xc) = 1;
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)ResultsScreen_State_FadeOutExit;
    }
}

#pragma thumb reset
