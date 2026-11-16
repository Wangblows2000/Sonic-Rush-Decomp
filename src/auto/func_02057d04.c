/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_0201c070();

void func_02057d04(u8 *p0, s32 p1) {
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x4) & 0x10000000);
    if (t0 == 0) {
        s32 v1 = (s32)p1;
        if ((s32)p1 < 0) {
            v1 = 0;
        }
        s32 t2 = *(u32 *)((u8 *)p0 + 0x34);
        if (t2 != 0) {
            func_0201c070(((u8 *)p0 + 0x34), v1, t2, t0);
        }
    }
}

#pragma thumb reset
