/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_StepXTowardRightEdge();
s32 ExtraBoss_HandBoss_SelectNextAttack();

void ExtraBossObj_State_HandBossWait(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x1c8) = (*(u32 *)((u8 *)p0 + 0x1c8) - 0x1);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x1c8);
    if (t0 > 0) {
        ExtraBoss_StepXTowardRightEdge(p0, t0);
        return;
    }
    ExtraBoss_HandBoss_SelectNextAttack();
}
