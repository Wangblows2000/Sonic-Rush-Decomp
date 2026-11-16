/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_02302e2c();
s32 func_ov000_02302f54();

void func_ov000_023032d4(s32 p0, s32 p1) {
    s32 t0 = func_ov000_02302e2c();
    if (t0 == 0) {
        return;
    }
    func_ov000_02302f54(p0, p1);
}
