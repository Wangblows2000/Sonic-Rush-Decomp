/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 TitleMenu_Create();
s32 TitleMenu_OpenPlayerSelectPage();
s32 TitleMenu_OpenRootPage();
s32 TitleMenu_CreateFadeTask();
s32 TitleMenu_Unk_ov001_022d74dc();

void TitleMenu_StartAtPlayerSelectItem0(void) {
    s32 t0 = TitleMenu_Create();
    if (*(u32 *)((u8 *)t0 + 0x8) != 0) {
        TitleMenu_OpenPlayerSelectPage(t0, 1, 0);
    } else {
        TitleMenu_OpenRootPage(t0, 0, 0);
    }
    TitleMenu_CreateFadeTask(8, 0);
    TitleMenu_Unk_ov001_022d74dc(t0, 0);
    *(volatile u32 *)0x4000000 = ((*(volatile u32 *)0x4000000 & -7937) | 0x1f00);
    *(volatile u32 *)0x4001000 = ((*(volatile u32 *)0x4001000 & -7937) | 0x1f00);
}

#pragma thumb reset
