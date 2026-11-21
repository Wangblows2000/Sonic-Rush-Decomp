/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void SpecialStagePlayer_ClampTilt(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x134);
    if (t0 > 0x8000) {
        *(u32 *)((u8 *)p0 + 0x134) = (t0 - 0x10000);
    }
    s32 t1 = *(u32 *)((u8 *)p0 + 0x134);
    if (t1 < -0x4000) {
        *(u32 *)((u8 *)p0 + 0x134) = -0x4000;
        return;
    }
    if (t1 > 0x4000) {
        *(u32 *)((u8 *)p0 + 0x134) = 0x4000;
    }
}
