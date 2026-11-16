/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02007a04();
s32 func_02007994();
s32 func_02007b3c();
s32 func_01ff85cc();

void func_02007f24(s32 p0, s32 p1, s32 p2, s32 p3) {
    func_02007a04(p0, 0x10000000);
    func_02007994(p0, p1, p3, 0);
    if ((s32)p3 == 0) {
        return;
    }
    func_02007b3c(p0);
    func_01ff85cc(p0, p1, p2, (0x96600000 | ((u32)(s32)p3 >> 2)));
}
