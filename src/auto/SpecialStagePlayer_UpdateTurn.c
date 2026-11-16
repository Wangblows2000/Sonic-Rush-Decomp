/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SpecialStagePlayer_IsAngleWithin90();
s32 SpecialStagePlayer_TurnTowards();

void SpecialStagePlayer_UpdateTurn(u8 *p0) {
    s32 t0 = SpecialStagePlayer_IsAngleWithin90(*(u16 *)((u8 *)p0 + 0x16));
    if (t0 == 1) {
        SpecialStagePlayer_TurnTowards(p0, *(u32 *)((u8 *)p0 + 0x134));
    }
    s32 t1 = *(u32 *)((u8 *)p0 + 0x44);
    if (t1 > 0x708) {
        *(u32 *)((u8 *)p0 + 0x44) = 0x708;
        return;
    }
    if (t1 < -1800) {
        *(u32 *)((u8 *)p0 + 0x44) = -1800;
    }
}
