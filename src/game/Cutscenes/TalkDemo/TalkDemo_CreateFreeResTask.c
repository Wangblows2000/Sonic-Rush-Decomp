/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void TalkDemoFreeRes_Main();
s32 Task_Create();

void TalkDemo_CreateFreeResTask(s32 p0) {
    s32 t0 = Task_Create((u32)TalkDemoFreeRes_Main, 0, 7, 0xf000, 3, 4);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x10);
    *(u32 *)t1 = p0;
}

#pragma thumb reset
