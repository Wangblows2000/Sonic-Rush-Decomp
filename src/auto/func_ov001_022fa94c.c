/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_022fd428();

void func_ov001_022fa94c(s32 p0, s32 p1, s32 p2, s32 p3) {
    s32 t0 = func_ov001_022fd428(0x12, p0, p1, p2, p3);
    if (t0 != 0) {
        *(u32 *)((u8 *)t0 + 0x14) = (*(u32 *)((u8 *)t0 + 0x14) & ~1);
    }
}
