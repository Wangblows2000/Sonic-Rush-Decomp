/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 Records_EaseInterp();
s32 Records_Unk_ov001_022ed490();
s32 Records_Unk_ov001_022ed3e0();
s32 Records_DrawBackButton();
s32 Records_Unk_ov001_022ed19c();
s32 Records_DrawHeaderName();
s32 Records_DrawRecordNames();
s32 Records_DrawHeaderTime();
s32 Records_DrawRecordTimes();
void Records_State_Unk_ov001_022ed6c4();

void Records_State_Enter(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = *(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    if (t1 >= 0) {
        s32 t2 = Records_EaseInterp(-160, 0, 0x10, t1, 0x2000);
        s32 t3 = Records_EaseInterp(0x90, 0x90, 0x10, t1, 0x2000);
        Records_Unk_ov001_022ed490(t0, t2, t3);
    }
    Records_Unk_ov001_022ed3e0(t0);
    Records_DrawBackButton(t0, 0);
    Records_Unk_ov001_022ed19c(t0);
    Records_DrawHeaderName(t0);
    Records_DrawRecordNames(t0);
    Records_DrawHeaderTime(t0);
    Records_DrawRecordTimes(t0);
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    if ((u32)*(u32 *)t0 >= (u32)0x10) {
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)Records_State_Unk_ov001_022ed6c4;
    }
}

#pragma thumb reset
