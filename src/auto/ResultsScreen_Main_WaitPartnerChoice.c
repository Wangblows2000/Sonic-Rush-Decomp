/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 ResultsScreen_GetPartnerChoice();
s32 ResultsScreen_HasPartnerDecided();
s32 ResultsScreen_DrawHeader();
s32 ResultsScreen_DrawSprite2();
s32 ResultsScreen_DrawSprite1Pair();
s32 ResultsScreen_DrawModels();
s32 ResultsScreen_DrawWinLoseSprites();
s32 ResultsScreen_DrawChoiceSprites();
s32 ResultsScreen_DrawSprite16();
s32 WirelessSession_GetPartnerRecvBuffer();
s32 MI_CpuCopy8();
s32 SysSound_PlaySE();
void ResultsScreen_Main_Unk_ov000_0235e5b4();

void ResultsScreen_Main_WaitPartnerChoice(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = ResultsScreen_GetPartnerChoice(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    *(u32 *)((u8 *)t0 + 0x8) = t1;
    s32 t2 = ResultsScreen_HasPartnerDecided(t0);
    ResultsScreen_DrawHeader(t0);
    ResultsScreen_DrawSprite2(t0);
    ResultsScreen_DrawSprite1Pair(t0);
    ResultsScreen_DrawModels(t0);
    ResultsScreen_DrawWinLoseSprites(t0, *(u32 *)((u8 *)t0 + 0x20), 0x1000);
    ResultsScreen_DrawChoiceSprites(t0, 0);
    ResultsScreen_DrawSprite16(t0);
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    s32 t3 = (*(u32 *)((u8 *)t0 + 0x4) + 0x1);
    *(u32 *)((u8 *)t0 + 0x4) = t3;
    if (t2 != 0) {
        s32 t4 = WirelessSession_GetPartnerRecvBuffer(t3);
        MI_CpuCopy8((t4 + 0x6), (t0 + 0xc14), 6);
        SysSound_PlaySE(7);
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)ResultsScreen_Main_Unk_ov000_0235e5b4;
    }
}

#pragma thumb reset
