/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Background_Init();
s32 Background_Draw();

void TitleDemo_InitBackgrounds(s32 p0) {
    Background_Init(((u8 *)p0 + 0x3cc), *(u32 *)((u8 *)p0 + 0x498), 0x38, 0, 2, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x498) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x498) + 0x1a));
    Background_Draw(((u8 *)p0 + 0x3cc));
    Background_Init(((u8 *)p0 + 0x40c), *(u32 *)((u8 *)p0 + 0x49c), 0x38, 0, 3, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x49c) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x49c) + 0x1a));
    Background_Draw(((u8 *)p0 + 0x40c));
    Background_Init(((u8 *)p0 + 0x44c), *(u32 *)((u8 *)p0 + 0x4a0), 0x38, 1, 3, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x4a0) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x4a0) + 0x1a));
    Background_Draw(((u8 *)p0 + 0x44c));
}

#pragma thumb reset
