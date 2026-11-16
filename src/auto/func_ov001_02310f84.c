/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_02310f54();
s32 func_ov001_0230d6e0();
s32 func_ov001_023122d8();

void func_ov001_02310f84(s32 p0) {
    func_ov001_02310f54();
    s32 t0 = func_ov001_0230d6e0();
    if (t0 != 0) {
        return;
    }
    func_ov001_023122d8(p0, 0x1a);
}
