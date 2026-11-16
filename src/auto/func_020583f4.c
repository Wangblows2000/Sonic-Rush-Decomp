/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02058898();
s32 func_020588bc();
s32 func_0202a1a0();

s32 func_020583f4(s32 p0, s32 p1) {
    s32 t0 = func_02058898();
    if (t0 == 0) {
        return 0;
    }
    s32 t1 = func_02058898(p1);
    if (t1 == 0) {
        return 0;
    }
    s32 t2 = func_020588bc(p0);
    s32 t3 = func_020588bc(p1);
    if (t2 != t3) {
        return 0;
    }
    s32 t4 = func_0202a1a0(p0, p1, 0x314);
    return (t4 == 0);
}
