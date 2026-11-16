/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_020683e4();
s32 func_02039bb0();

void func_0206c35c(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x90);
    if (t0 != 0) {
        func_020683e4(t0, 0);
        func_020683e4((*(u32 *)((u8 *)p0 + 0x90) + 0x8), 1);
        return;
    }
    s32 t1 = *(u32 *)((u8 *)p0 + 0xc8);
    if (t1 != 0) {
        func_02039bb0(0, t1);
    }
    *(u32 *)((u8 *)p0 + 0xc8) = 0;
    s32 t2 = *(u32 *)((u8 *)p0 + 0x110);
    if (t2 != 0) {
        func_02039bb0(1, t2);
    }
    *(u32 *)((u8 *)p0 + 0x110) = 0;
}
