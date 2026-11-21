/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void ResultsScreen_LinkIcon_Main();
s32 Task_Create();

s32 ResultsScreen_CreateLinkIcon(s32 p0) {
    s32 t0 = Task_Create((u32)ResultsScreen_LinkIcon_Main, 0, 0, 0x3000, 0x1000, 8);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x10);
    *(u32 *)t1 = p0;
    *(u32 *)((u8 *)t1 + 0x4) = -1;
    return t0;
}

#pragma thumb reset
