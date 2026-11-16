/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 MapCollision_ProbeDistance();
s32 KuragenBomb_Action_Explode();

void KuragenBomb_State_Fall(u8 *p0) {
    s32 t0 = MapCollision_ProbeDistance(((s32)*(u32 *)((u8 *)p0 + 0x10) >> 8), ((s32)*(u32 *)((u8 *)p0 + 0x14) >> 8), 1, 2, 0, 0);
    if (t0 >= 0) {
        return;
    }
    KuragenBomb_Action_Explode(p0);
}
