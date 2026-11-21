/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 TimeSelectMenu_DrawLayoutA();
s32 TimeSelectMenu_DrawLayoutB();
s32 TimeSelectMenu_DrawTransferSprites();
s32 TimeSelectMenu_IsTransferDone();
s32 Wireless_EnableErrorCallback();
s32 TimeSelectMenu_StartBrightnessFade();
void TimeSelectMenu_State_StartStage();

void TimeSelectMenu_State_ChooserWaitTransfer(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    TimeSelectMenu_DrawLayoutA(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    TimeSelectMenu_DrawLayoutB(t0, 0x1000, 0);
    TimeSelectMenu_DrawTransferSprites(t0);
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    s32 t1 = TimeSelectMenu_IsTransferDone(t0);
    if (t1 != 0) {
        *(u32 *)t0 = 0;
        Wireless_EnableErrorCallback(0);
        TimeSelectMenu_StartBrightnessFade(0, 0x10, 0x10, 0);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)TimeSelectMenu_State_StartStage;
    }
}

#pragma thumb reset
