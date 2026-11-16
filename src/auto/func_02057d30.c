/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_0201c070();

void func_02057d30(u8 *p0, s32 p1) {
    if ((*(u32 *)((u8 *)p0 + 0x4) & 0x10000000) == 0) {
        s32 v0 = (s32)p1;
        if ((s32)p1 < 0) {
            v0 = 0;
        }
        func_0201c070(((u8 *)p0 + 0x30), v0);
    }
}

#pragma thumb reset
