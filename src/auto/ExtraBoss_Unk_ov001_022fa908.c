/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBossPlayer_CreateFxObj();

void ExtraBoss_Unk_ov001_022fa908(s32 p0, s32 p1, s32 p2, s32 p3) {
    s32 t0 = ExtraBossPlayer_CreateFxObj(0x13, p0, p1, p2, p3);
    if (t0 != 0) {
        *(u32 *)((u8 *)t0 + 0x14) = (*(u32 *)((u8 *)t0 + 0x14) & ~1);
    }
}
