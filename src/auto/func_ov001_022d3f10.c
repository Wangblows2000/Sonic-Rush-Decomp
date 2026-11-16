/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Background_Init();
s32 Background_Draw();

void func_ov001_022d3f10(s32 p0) {
    Background_Init(((u8 *)p0 + 0x1d4), *(u32 *)((u8 *)p0 + 0x5e8), 0x38, 0, 1, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x5e8) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x5e8) + 0x1a));
    Background_Draw(((u8 *)p0 + 0x1d4));
    Background_Init(((u8 *)p0 + 0x214), *(u32 *)((u8 *)p0 + 0x5e8), 0x38, 1, 1, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x5e8) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x5e8) + 0x1a));
    Background_Draw(((u8 *)p0 + 0x214));
    Background_Init(((u8 *)p0 + 0x254), *(u32 *)((u8 *)p0 + 0x5ec), 0x38, 1, 0, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x5ec) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x5ec) + 0x1a));
    Background_Draw(((u8 *)p0 + 0x254));
    Background_Init(((u8 *)p0 + 0x294), *(u32 *)((u8 *)p0 + 0x5f0), 0x38, 0, 0, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x5f0) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x5f0) + 0x1a));
    Background_Draw(((u8 *)p0 + 0x294));
}

#pragma thumb reset
