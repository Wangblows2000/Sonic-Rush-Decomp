/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gDisplayMain[];
void func_02047378();

void func_020473d8(s32 p0, u8 *p1) {
    *(u32 *)((u8 *)p0 + 0x344) = (*(u32 *)((u8 *)p0 + 0x344) & ~0x10);
    *(u16 *)((u8 *)p1 + 0xc) = 0;
    *(u8 *)((u8 *)gDisplayMain + 0x11) = 0;
    *(u8 *)((u8 *)gDisplayMain + 0x10) = 1;
    *(u8 *)((u8 *)gDisplayMain + 0x15) = 0x21;
    *(u8 *)((u8 *)gDisplayMain + 0x14) = (*(u8 *)((u8 *)gDisplayMain + 0x15) + 0x2);
    *(u8 *)((u8 *)gDisplayMain + 0x18) = ((*(u8 *)((u8 *)gDisplayMain + 0x18) & ~1) | 1);
    *(u8 *)((u8 *)gDisplayMain + 0x18) = (*(u8 *)((u8 *)gDisplayMain + 0x18) | 2);
    *(u8 *)((u8 *)gDisplayMain + 0x18) = (*(u8 *)((u8 *)gDisplayMain + 0x18) & ~4);
    *(u8 *)((u8 *)gDisplayMain + 0x18) = (*(u8 *)((u8 *)gDisplayMain + 0x18) & ~8);
    *(u8 *)((u8 *)gDisplayMain + 0x18) = (*(u8 *)((u8 *)gDisplayMain + 0x18) | 0x10);
    *(u8 *)((u8 *)gDisplayMain + 0x1a) = (*(u8 *)((u8 *)gDisplayMain + 0x1a) & ~1);
    *(u8 *)((u8 *)gDisplayMain + 0x1a) = (*(u8 *)((u8 *)gDisplayMain + 0x1a) & ~2);
    *(u8 *)((u8 *)gDisplayMain + 0x1a) = (*(u8 *)((u8 *)gDisplayMain + 0x1a) | 4);
    *(u8 *)((u8 *)gDisplayMain + 0x1a) = (*(u8 *)((u8 *)gDisplayMain + 0x1a) | 8);
    *(u8 *)((u8 *)gDisplayMain + 0x1a) = (*(u8 *)((u8 *)gDisplayMain + 0x1a) | 0x10);
    *(u32 *)((u8 *)gDisplayMain + 0x1c) = 1;
    *(u32 *)p1 = (u32)func_02047378;
}

#pragma thumb reset
