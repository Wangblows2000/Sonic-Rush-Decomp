/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_02312138();
s32 func_ov001_023122d8();
s32 func_ov001_02312258();

void func_ov001_02311abc(u8 *p0) {
    s32 t0 = func_ov001_02312138();
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0x18) = 0;
        func_ov001_023122d8(p0, 5, 0);
        return;
    }
    func_ov001_02312258(p0);
}
