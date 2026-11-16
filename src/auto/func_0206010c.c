/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gDisplayMain[];
void func_020600e0();

void func_0206010c(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x8) = (*(s16 *)((u8 *)p0 + 8) + 0x1);
    *(u8 *)((u8 *)gDisplayMain + 0x11) = ((-(*(s16 *)((u8 *)p0 + 8) << 8) / 0x14) + 0x100);
    *(volatile u16 *)0x5000000 = 0;
    if (*(s16 *)((u8 *)p0 + 8) >= 0x14) {
        *(u32 *)p0 = (u32)func_020600e0;
    }
}

#pragma thumb reset
