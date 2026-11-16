/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossF_StartNextAttack();
s32 BossFPartD8_RollThrowVariant();
s32 BossFPartD8_TriggerPartDAThrow();

void BossFPartD8_State_PartDEAttackEndWait(u8 *p0) {
    s32 t0 = (*(u8 *)((u8 *)p0 + 0x57c) & 2);
    if (t0 == 0) {
        return;
    }
    s32 t1 = *(u32 *)((u8 *)p0 + 0x434);
    if (t1 != 0) {
        BossF_StartNextAttack(t1, t0);
        return;
    }
    BossFPartD8_RollThrowVariant();
    BossFPartD8_TriggerPartDAThrow();
}
