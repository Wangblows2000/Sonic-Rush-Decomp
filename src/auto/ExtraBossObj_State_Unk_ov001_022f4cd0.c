/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_WeaponBoss_SelectNextAttack();

void ExtraBossObj_State_Unk_ov001_022f4cd0(u8 *p0) {
    if ((*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x84) + 0x10c) & 2) == 0) {
        return;
    }
    ExtraBoss_WeaponBoss_SelectNextAttack(p0, (*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x84) + 0x10c) & 2));
    *(u32 *)((u8 *)p0 + 0xb8) = (*(u32 *)((u8 *)p0 + 0xb8) & ~0x200);
}
