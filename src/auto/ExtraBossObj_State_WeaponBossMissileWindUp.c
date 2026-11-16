/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void ExtraBossObj_State_WeaponBossMissileReady();
extern u8 data_ov001_0231f068[];
s32 Model_SetAnimation();

void ExtraBossObj_State_WeaponBossMissileWindUp(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x84);
    if ((*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x84) + 0x10c) & 2) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x74) = (u32)ExtraBossObj_State_WeaponBossMissileReady;
    Model_SetAnimation(t0, 0, *(u32 *)data_ov001_0231f068, 0x14, 0);
}
