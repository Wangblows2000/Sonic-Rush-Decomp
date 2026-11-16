/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022fad3c();

void func_ov000_023029b8(s32 p0, s32 p1, s32 p2, s32 p3) {
    s32 v0 = (s32)p1;
    if ((s32)p1 != 0xe8) {
        s32 v1 = v0;
        if (v0 != 0xe9) {
            s32 v2 = v1;
            if (v1 != 0xea) {
                v2 = 0xea;
            }
            v1 = v2;
        }
        v0 = v1;
    }
    func_ov000_022fad3c(((s32)(v0 << 0x10) >> 0x10), p2, p3, 0, 0, 0, 0, 0, 0);
}
