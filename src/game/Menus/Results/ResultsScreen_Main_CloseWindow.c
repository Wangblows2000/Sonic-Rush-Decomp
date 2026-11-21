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
s32 ResultsScreen_DisableLineScroll();
void ResultsScreen_State_Choose();
void ResultsScreen_Main_WaitPartnerChoice();

void ResultsScreen_Main_CloseWindow(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    ResultsScreen_DrawHeader(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    ResultsScreen_DrawSprite2(t0);
    ResultsScreen_DrawSprite1Pair(t0);
    ResultsScreen_DrawModels(t0);
    ResultsScreen_DrawWinLoseSprites(t0, *(u32 *)((u8 *)t0 + 0x20), 0x1000);
    s32 t1 = ResultsScreen_EaseValue(0x44, 0, 0xa, *(u32 *)t0, 0);
    ResultsScreen_SetWindowLineScroll(t0, t1);
    if (*(u32 *)((u8 *)t0 + 0x10) == 0) {
        ResultsScreen_DrawSprite16(t0);
    }
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    if ((u32)*(u32 *)t0 >= (u32)0xa) {
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)t0 + 0x4) = 0x64;
        *(u32 *)((u8 *)t0 + 0x8) = 0;
        ResultsScreen_DisableLineScroll(t0, 0);
        if (*(u32 *)((u8 *)t0 + 0x10) != 0) {
            *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)ResultsScreen_State_Choose;
            return;
        }
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)ResultsScreen_Main_WaitPartnerChoice;
    }
}

#pragma thumb reset
