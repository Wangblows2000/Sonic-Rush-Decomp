/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Background_Init();
s32 Background_Draw();

void func_ov001_022eda54(s32 p0) {
    Background_Init(((u8 *)p0 + 0x7a0), *(u32 *)((u8 *)p0 + 0x7f8), 0x38, 1, 3, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x7f8) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x7f8) + 0x1a));
    Background_Draw(((u8 *)p0 + 0x7a0));
    Background_Init(((u8 *)p0 + 0x760), *(u32 *)((u8 *)p0 + 0x7f8), 0x38, 0, 3, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x7f8) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x7f8) + 0x1a));
    Background_Draw(((u8 *)p0 + 0x760));
    Background_Init(((u8 *)p0 + 0x720), *(u32 *)((u8 *)p0 + 0x7f4), 0x38, 1, 2, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x7f4) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x7f4) + 0x1a));
    Background_Draw(((u8 *)p0 + 0x720));
    Background_Init(((u8 *)p0 + 0x6a0), *(u32 *)((u8 *)p0 + 0x7ec), 0x38, 0, 2, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x7ec) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x7ec) + 0x1a));
    Background_Draw(((u8 *)p0 + 0x6a0));
    Background_Init(((u8 *)p0 + 0x6e0), *(u32 *)((u8 *)p0 + 0x7f0), 0x38, 0, 1, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x7f0) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x7f0) + 0x1a));
    Background_Draw(((u8 *)p0 + 0x6e0));
}

#pragma thumb reset
