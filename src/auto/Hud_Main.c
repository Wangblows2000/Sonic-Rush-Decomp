/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 Hud_ToDecimalDigits();
s32 Hud_DrawRings();
s32 Hud_FramesToTimeDigits();
s32 Hud_DrawTime();
s32 Hud_Unk_ov000_023367ac();
s32 Hud_DrawTensionGauge();
s32 Hud_Unk_ov000_02335ee0();

void Hud_Main(void) {
    s32 t0 = (*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x290);
    s32 t1 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    *(u32 *)((u8 *)t1 + 0x294) = *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x290);
    Hud_ToDecimalDigits(t0, *(u32 *)((u8 *)t1 + 0x28c), 0x3e8);
    if ((*(u32 *)t1 & 2) == 0) {
        Hud_DrawRings(t1);
    }
    *(u32 *)((u8 *)t1 + 0x684) = *(u32 *)((u8 *)t1 + 0x680);
    Hud_FramesToTimeDigits((t1 + 0x680), *(u32 *)((u8 *)t1 + 0x67c));
    if ((*(u32 *)t1 & 1) == 0) {
        Hud_DrawTime(t1);
    }
    Hud_Unk_ov000_023367ac(t1);
    if ((*(u32 *)t1 & 4) == 0) {
        Hud_DrawTensionGauge(t1);
    }
    *(u32 *)((u8 *)t1 + 0xaa4) = *(u32 *)((u8 *)t1 + 0xaa0);
    Hud_ToDecimalDigits((t1 + 0xaa0), *(u16 *)((u8 *)(t1 + 0x900) + 0x28), 0xa);
    if ((*(u32 *)t1 & 8) != 0) {
        return;
    }
    Hud_Unk_ov000_02335ee0(t1);
}
