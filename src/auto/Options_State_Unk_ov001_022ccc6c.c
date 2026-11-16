/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 OptionsMenu_UpdateAnims();
s32 OptionsMenu_DrawItems();
s32 Task_Destroy();

void Options_State_Unk_ov001_022ccc6c(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = OptionsMenu_UpdateAnims(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    OptionsMenu_DrawItems(t0);
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    if (t1 != 0) {
        if ((u32)*(u32 *)t0 >= (u32)0x2e) {
            *(u32 *)*(u32 *)(t0 + 0x3ac) = 0xa;
            *(u32 *)((u8 *)*(u32 *)(t0 + 0x3ac) + 0x4) = 0;
            Task_Destroy(*(u32 *)gTaskCurrent, 0, 0xa);
        }
    }
}

#pragma thumb reset
