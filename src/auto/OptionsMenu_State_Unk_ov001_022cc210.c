/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 OptionsMenu_StopWireless();
s32 OptionsMenu_UpdateAnims();
s32 OptionsMenu_DrawItems();
s32 OptionsMenu_Unk_ov001_022cdd28();

void OptionsMenu_State_Unk_ov001_022cc210(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = (*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x1ec);
    OptionsMenu_StopWireless((*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x1ec));
    OptionsMenu_UpdateAnims(t0);
    OptionsMenu_DrawItems(t0);
    *(u32 *)(t0 + 0x1ec) = (*(u32 *)(t0 + 0x1ec) + 0x1);
    *(u32 *)((u8 *)t0 + 0x14) = (*(u32 *)((u8 *)t0 + 0x14) + 0x1);
    *(u32 *)((u8 *)t0 + 0x18) = (*(u32 *)((u8 *)t0 + 0x18) + 0x1);
    *(u32 *)(t0 + 0x1ec) = 0;
    *(u32 *)((u8 *)t0 + 0x14) = 0;
    *(u32 *)((u8 *)t0 + 0x18) = 0;
    *(u32 *)((u8 *)t1 + 0x4) = 0xa;
    OptionsMenu_Unk_ov001_022cdd28(0xa, 0);
}

#pragma thumb reset
