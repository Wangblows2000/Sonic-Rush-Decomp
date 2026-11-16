/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 OptionsMenu_UpdateAnims();
s32 OptionsMenu_DrawItems();
s32 OptionsMenu_ShowMsgBox0();
void OptionsMenu_State_Unk_ov001_022cc8ec();

void OptionsMenu_State_Unk_ov001_022cc998(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = (*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x1ec);
    *(u32 *)((u8 *)t1 + 0x10) = 0;
    *(u32 *)((u8 *)t1 + 0x14) = 0;
    *(u32 *)((u8 *)t1 + 0x8) = 0;
    OptionsMenu_UpdateAnims(t0);
    OptionsMenu_DrawItems(t0);
    *(u32 *)(t0 + 0x1ec) = (*(u32 *)(t0 + 0x1ec) + 0x1);
    *(u32 *)((u8 *)t0 + 0x14) = (*(u32 *)((u8 *)t0 + 0x14) + 0x1);
    s32 t2 = (*(u32 *)((u8 *)t0 + 0x18) + 0x1);
    *(u32 *)((u8 *)t0 + 0x18) = t2;
    if ((u32)*(u32 *)(t0 + 0x1ec) >= (u32)0x10) {
        *(u32 *)t1 = 0;
        *(u32 *)((u8 *)t0 + 0x18) = 0;
        *(u32 *)((u8 *)t1 + 0x4) = 3;
        OptionsMenu_ShowMsgBox0(3, t2);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)OptionsMenu_State_Unk_ov001_022cc8ec;
    }
}

#pragma thumb reset
