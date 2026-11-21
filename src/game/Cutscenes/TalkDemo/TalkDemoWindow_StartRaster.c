/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void TalkDemoWindowRaster_Main();
s32 Task_Create();

void TalkDemoWindow_StartRaster(u8 *p0) {
    Task_Create((u32)TalkDemoWindowRaster_Main, 0, 7, 0xf00a, 3, 0xf8);
    *(u16 *)((u8 *)p0 + 0xec) = 0;
    *(u16 *)((u8 *)p0 + 0xee) = *(u16 *)((u8 *)p0 + 0x20);
    *(u16 *)((u8 *)p0 + 0xf0) = 0;
    *(u16 *)((u8 *)p0 + 0xf2) = 0;
    *(u8 *)((u8 *)p0 + 0xf4) = 0;
    *(u8 *)((u8 *)p0 + 0xf5) = 1;
    *(u8 *)((u8 *)p0 + 0xf6) = 0;
    *(u8 *)((u8 *)p0 + 0xf7) = 0;
}

#pragma thumb reset
