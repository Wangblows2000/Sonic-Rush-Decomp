/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f524[];
extern u8 data_ov001_0231f520[];
void ExtraBossObj_State_WeaponBossSwordRecover();
extern u8 data_ov001_0231d4c0[];
extern u8 data_ov001_0231f068[];
s32 Model_SetAnimation();
s32 ExtraBoss_AdvanceSwordFxStep();

void ExtraBossObj_State_WeaponBossSwordFollowThrough(u8 *p0) {
    s32 t0 = (*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1dc) + 0x10) + 0xe8);
    s32 t1 = *(u32 *)((u8 *)p0 + 0x1dc);
    s32 t2 = *(u32 *)((u8 *)p0 + 0x84);
    if (*(u32 *)((u8 *)p0 + 0x1cc) == 2) {
        *(u32 *)((u8 *)t0 + 0x28) = (*(u32 *)((u8 *)t1 + 0xe0) - *(u32 *)data_ov001_0231f524);
        *(u32 *)((u8 *)t0 + 0x2c) = -(*(u32 *)((u8 *)t1 + 0xe4) + *(u32 *)data_ov001_0231f520);
        *(u32 *)((u8 *)t0 + 0x30) = *(u32 *)((u8 *)t1 + 0xe8);
    } else {
        *(u32 *)((u8 *)t0 + 0x28) = (*(u32 *)((u8 *)t1 + 0x134) - *(u32 *)data_ov001_0231f524);
        *(u32 *)((u8 *)t0 + 0x2c) = -(*(u32 *)((u8 *)t1 + 0x138) + *(u32 *)data_ov001_0231f520);
        *(u32 *)((u8 *)t0 + 0x30) = *(u32 *)((u8 *)t1 + 0x13c);
    }
    if ((*(u8 *)((u8 *)t2 + 0x10c) & 2) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x74) = (u32)ExtraBossObj_State_WeaponBossSwordRecover;
    Model_SetAnimation(t2, 0, *(u32 *)data_ov001_0231f068, (*(u32 *)((u8 *)data_ov001_0231d4c0 + (*(u32 *)((u8 *)p0 + 0x1cc) << 2)) + 0x3), 0);
    *(u32 *)((u8 *)t2 + 0x114) = 0x1000;
    ExtraBoss_AdvanceSwordFxStep(*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1dc) + 0x10));
    *(u32 *)((u8 *)p0 + 0x50) = 0;
}
