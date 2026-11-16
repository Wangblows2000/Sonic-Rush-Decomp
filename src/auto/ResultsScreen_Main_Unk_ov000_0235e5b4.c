/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 ResultsScreen_DrawHeader();
s32 ResultsScreen_DrawSprite2();
s32 ResultsScreen_DrawSprite1Pair();
s32 ResultsScreen_DrawModels();
s32 ResultsScreen_DrawWinLoseSprites();
s32 ResultsScreen_DrawChoiceSprites();
s32 ResultsScreen_DrawSprite16();
s32 ResultsScreen_EnableLineScroll();
void ResultsScreen_Main_OpenWindow();

void ResultsScreen_Main_Unk_ov000_0235e5b4(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    ResultsScreen_DrawHeader(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    ResultsScreen_DrawSprite2(t0);
    ResultsScreen_DrawSprite1Pair(t0);
    ResultsScreen_DrawModels(t0);
    ResultsScreen_DrawWinLoseSprites(t0, *(u32 *)((u8 *)t0 + 0x20), 0x1000);
    ResultsScreen_DrawChoiceSprites(t0, 1);
    ResultsScreen_DrawSprite16(t0);
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    if ((u32)*(u32 *)t0 >= (u32)0x10) {
        *(u32 *)t0 = 0;
        ResultsScreen_EnableLineScroll(t0);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)ResultsScreen_Main_OpenWindow;
    }
}

#pragma thumb reset
