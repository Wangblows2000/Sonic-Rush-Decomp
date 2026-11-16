/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gDisplayMain[];
void func_0205feb8();

void func_0205fee4(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x8) = (*(s16 *)((u8 *)p0 + 8) + 0x1);
    *(u8 *)((u8 *)gDisplayMain + 0x11) = ((-(*(s16 *)((u8 *)p0 + 8) << 8) / 0xa) + 0x100);
    *(volatile u16 *)0x5000000 = 0;
    if (*(s16 *)((u8 *)p0 + 8) >= 0xa) {
        *(u32 *)p0 = (u32)func_0205feb8;
    }
}

#pragma thumb reset
