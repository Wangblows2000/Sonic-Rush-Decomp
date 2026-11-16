/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_HandBoss_SelectNextAttack();

void ExtraBossObj_State_HandBossClawEnd(u8 *p0) {
    s32 t0 = (*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x84) + 0x10c) & 2);
    if (t0 == 0) {
        return;
    }
    ExtraBoss_HandBoss_SelectNextAttack(p0, t0);
}
