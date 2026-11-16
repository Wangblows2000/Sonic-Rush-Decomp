/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void TitleMenuVsMode2Wait_Main();
s32 Task_Create();
s32 func_ov001_022d7340();
s32 VsMenu_CreateMode2();

void TitleMenu_SelectVsMode2(s32 p0) {
    s32 t0 = Task_Create((u32)TitleMenuVsMode2Wait_Main, 0, 0, 0x2000, 0x1000, 4);
    *(u32 *)*(u32 *)((u8 *)t0 + 0x10) = p0;
    func_ov001_022d7340(p0);
    VsMenu_CreateMode2(*(u32 *)((u8 *)p0 + 0x808), *(u32 *)((u8 *)p0 + 0x80c));
}

#pragma thumb reset
