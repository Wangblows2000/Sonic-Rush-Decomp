/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossFPartDE_SpawnBeamFinEffects();
void BossFPartDE_State_StartSweep();

void BossFPartDE_State_Pause(u8 *p0) {
    BossFPartDE_SpawnBeamFinEffects();
    *(u16 *)((u8 *)p0 + 0x210) = (*(u16 *)((u8 *)p0 + 0x210) + 0x800);
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x4ae) + 0x1);
    s32 t1 = *(u16 *)((u8 *)p0 + 0x4ae);
    *(u16 *)((u8 *)p0 + 0x4ae) = t0;
    if ((u32)t1 > (u32)0x1e) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartDE_State_StartSweep;
    }
}
