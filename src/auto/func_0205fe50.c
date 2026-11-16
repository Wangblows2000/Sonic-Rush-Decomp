/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gDisplayMain[];
void func_0205fe1c();
s32 TalkDemo_SetFlags();

void func_0205fe50(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x8) = (*(s16 *)((u8 *)p0 + 8) + 0x1);
    *(u8 *)((u8 *)p0 + 0x16) = (((-31 * *(s16 *)((u8 *)p0 + 8)) / 0xa) + 0x60);
    *(u8 *)((u8 *)gDisplayMain + 0x15) = *(u8 *)((u8 *)p0 + 0x16);
    *(u8 *)((u8 *)gDisplayMain + 0x17) = *(u8 *)((u8 *)p0 + 0x16);
    *(u8 *)((u8 *)p0 + 0x17) = (((0x1b * *(s16 *)((u8 *)p0 + 8)) / 0xa) + 0x63);
    *(u8 *)((u8 *)gDisplayMain + 0x14) = *(u8 *)((u8 *)p0 + 0x17);
    *(u8 *)((u8 *)gDisplayMain + 0x16) = *(u8 *)((u8 *)p0 + 0x17);
    if (*(s16 *)((u8 *)p0 + 8) >= 0xa) {
        *(u32 *)p0 = (u32)func_0205fe1c;
        TalkDemo_SetFlags(1, 4);
    }
}

#pragma thumb reset
