/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_022fd428();
s32 ExtraBoss_GetPlayer();

void func_ov001_022fb764(s32 p0, s32 p1, s32 p2, s32 p3) {
    s32 t0 = func_ov001_022fd428(0xb, p0, p1, p2, p3);
    if (t0 == 0) {
        return;
    }
    s32 t1 = ExtraBoss_GetPlayer();
    *(u32 *)((u8 *)t0 + 0x80) = t1;
}
