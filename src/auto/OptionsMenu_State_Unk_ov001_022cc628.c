/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 OptionsMenu_UpdateAnims();
s32 OptionsMenu_DrawItems();
s32 OptionsMenu_Unk_ov001_022c8d54();
void OptionsMenu_State_Unk_ov001_022cc7fc();
s32 OptionsMenu_Unk_ov001_022cdea8();
void OptionsMenu_State_Unk_ov001_022cc588();

void OptionsMenu_State_Unk_ov001_022cc628(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = (*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x1ec);
    OptionsMenu_UpdateAnims(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    OptionsMenu_DrawItems(t0);
    s32 t2 = (*(u32 *)(t0 + 0x1ec) + 0x1);
    *(u32 *)(t0 + 0x1ec) = t2;
    *(u32 *)((u8 *)t0 + 0x14) = (*(u32 *)((u8 *)t0 + 0x14) + 0x1);
    *(u32 *)((u8 *)t0 + 0x18) = (*(u32 *)((u8 *)t0 + 0x18) + 0x1);
    s32 t3 = OptionsMenu_Unk_ov001_022c8d54(t1, t2);
    *(u32 *)((u8 *)t1 + 0x8) = t3;
    if (*(u32 *)((u8 *)t1 + 0x8) == 0) {
        *(u32 *)t1 = 0;
        *(u32 *)((u8 *)t0 + 0x14) = 0;
        *(u32 *)((u8 *)t0 + 0x18) = 0;
        *(u32 *)((u8 *)t1 + 0x4) = 4;
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)OptionsMenu_State_Unk_ov001_022cc7fc;
        return;
    }
    if ((u32)*(u32 *)t1 >= (u32)0x10) {
        *(u32 *)t1 = 0;
        *(u32 *)((u8 *)t0 + 0x14) = 0;
        *(u32 *)((u8 *)t0 + 0x18) = 0;
        *(u32 *)((u8 *)t1 + 0x4) = 7;
        OptionsMenu_Unk_ov001_022cdea8(7);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)OptionsMenu_State_Unk_ov001_022cc588;
    }
}

#pragma thumb reset
