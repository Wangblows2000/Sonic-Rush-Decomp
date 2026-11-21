/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossFPartDE_SpawnBeamFinEffects();
s32 BossFPartDE_UpdatePatrolSpeed();
void BossFPartDE_State_PickTarget();

void BossFPartDE_State_Patrol(u8 *p0) {
    BossFPartDE_SpawnBeamFinEffects();
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x210) + 0x800);
    *(u16 *)((u8 *)p0 + 0x210) = t0;
    s32 t1 = BossFPartDE_UpdatePatrolSpeed(p0, ((u8 *)p0 + 0x200), t0);
    if (t1 != 0) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartDE_State_PickTarget;
    }
}
