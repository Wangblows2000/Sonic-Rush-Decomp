/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Background_Init();
s32 Background_Draw();

void TitleScreen_InitBackgrounds(s32 p0) {
    Background_Init(((u8 *)p0 + 0xdc), *(u32 *)((u8 *)p0 + 0x168), 0x38, 0, 3, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x168) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x168) + 0x1a));
    Background_Draw(((u8 *)p0 + 0xdc));
    Background_Init(((u8 *)p0 + 0x11c), *(u32 *)((u8 *)p0 + 0x168), 0x38, 1, 3, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x168) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x168) + 0x1a));
    Background_Draw(((u8 *)p0 + 0x11c));
    Background_Init(((u8 *)p0 + 0x9c), *(u32 *)((u8 *)p0 + 0x164), 0x38, 0, 2, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x164) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x164) + 0x1a));
    Background_Draw(((u8 *)p0 + 0x9c));
}

#pragma thumb reset
