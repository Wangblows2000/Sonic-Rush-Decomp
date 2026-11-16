/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02059338();
s32 func_020593bc();
s32 func_0205a304();
s32 func_0205a27c();

s32 func_0205a37c(s32 p0) {
    s32 t0 = func_02059338();
    if (t0 == 0) {
        return 1;
    }
    s32 t1 = func_020593bc(t0);
    if (t1 == 0) {
        return 1;
    }
    func_0205a304(t1);
    if (t1 == 3) {
        func_0205a27c(3, p0);
    } else {
        func_0205a27c(1, 0);
    }
    return 0;
}
