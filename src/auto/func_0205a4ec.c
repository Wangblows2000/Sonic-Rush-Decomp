/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02059338();
s32 SaveData_Commit();
s32 func_0205a304();
s32 func_0205a27c();

s32 func_0205a4ec(s32 p0) {
    s32 t0 = func_02059338();
    if (t0 == 0) {
        if ((s32)p0 != 0) {
            ((s32 (*)())(s32)p0)();
        }
        return 1;
    }
    s32 t1 = SaveData_Commit(t0);
    if (t1 != 0) {
        func_0205a304();
        func_0205a27c(2, 0);
        return 0;
    }
    if ((s32)p0 != 0) {
        ((s32 (*)())(s32)p0)(t1);
    }
    return 1;
}
