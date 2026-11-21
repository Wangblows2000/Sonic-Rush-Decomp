/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void TitleMenuVsMode0Wait_Main();
s32 Task_Create();
s32 TitleMenu_AttachVsMenu();
s32 VsMenu_CreateMode0();

void TitleMenu_SelectVsMode0(s32 p0) {
    s32 t0 = Task_Create((u32)TitleMenuVsMode0Wait_Main, 0, 0, 0x2000, 0x1000, 4);
    *(u32 *)*(u32 *)((u8 *)t0 + 0x10) = p0;
    TitleMenu_AttachVsMenu(p0);
    VsMenu_CreateMode0(*(u32 *)((u8 *)p0 + 0x808), *(u32 *)((u8 *)p0 + 0x80c));
}

#pragma thumb reset
