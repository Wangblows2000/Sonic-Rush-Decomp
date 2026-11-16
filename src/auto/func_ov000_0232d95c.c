/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_020428d4();
s32 StageObj_SetAnimation();

void func_ov000_0232d95c(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x8);
    s32 t1 = func_020428d4();
    *(u32 *)((u8 *)p0 + 0x8) = t1;
    s32 t2 = *(u32 *)((u8 *)p0 + 0x8);
    if (t0 == t2) {
        return;
    }
    StageObj_SetAnimation(p0, ((u32)(t2 << 0x10) >> 0x10));
}
