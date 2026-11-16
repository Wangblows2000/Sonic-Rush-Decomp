/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 Records_Unk_ov001_022ed490();
s32 Records_Unk_ov001_022ed3e0();
s32 Records_DrawBackButton();
s32 Records_Unk_ov001_022ed19c();
s32 Records_Unk_ov001_022ed188();
s32 Records_Unk_ov001_022ed14c();
s32 Records_Unk_ov001_022ed104();
s32 Records_Unk_ov001_022ed08c();
s32 Records_EaseInterp();
s32 Records_IsUpArrowTouched();
s32 Records_IsDownArrowTouched();
void Records_State_Unk_ov001_022ed6c4();

void Records_State_Unk_ov001_022ed614(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    Records_Unk_ov001_022ed490(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10), 0, 0x90);
    Records_Unk_ov001_022ed3e0(t0);
    Records_DrawBackButton(t0, 0);
    Records_Unk_ov001_022ed19c(t0);
    Records_Unk_ov001_022ed188(t0);
    Records_Unk_ov001_022ed14c(t0);
    Records_Unk_ov001_022ed104(t0);
    Records_Unk_ov001_022ed08c(t0);
    s32 t1 = *(u32 *)t0;
    if (t1 >= 0) {
        s32 t2 = Records_EaseInterp(*(u32 *)((u8 *)t0 + 0x1c), *(u32 *)((u8 *)t0 + 0x20), 8, t1, 0x2000);
        *(u32 *)((u8 *)t0 + 0x30) = t2;
    }
    s32 t3 = Records_IsUpArrowTouched(t0);
    if (t3 != 0) {
        *(u32 *)((u8 *)t0 + 0x4) = 0;
        *(u32 *)((u8 *)t0 + 0x8) = 0;
    } else {
        s32 t4 = Records_IsDownArrowTouched(t0);
        if (t4 != 0) {
            *(u32 *)((u8 *)t0 + 0x4) = 1;
            *(u32 *)((u8 *)t0 + 0x8) = 1;
        } else {
            *(u32 *)((u8 *)t0 + 0x4) = 6;
            *(u32 *)((u8 *)t0 + 0x8) = 6;
        }
    }
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    if ((u32)*(u32 *)t0 >= (u32)8) {
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)t0 + 0x30) = *(u32 *)((u8 *)t0 + 0x20);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)Records_State_Unk_ov001_022ed6c4;
    }
}

#pragma thumb reset
