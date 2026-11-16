/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossF_UpdateCameraShake();
s32 BossFPartD8_RollThrowVariant();
s32 BossFPartD8_TriggerPartDAThrow();

void BossFPartD8_State_HandSlamHurtWait(u8 *p0) {
    BossF_UpdateCameraShake(((u8 *)p0 + 0x43a), 2, 6);
    if ((*(u8 *)((u8 *)p0 + 0x57c) & 2) == 0) {
        return;
    }
    BossFPartD8_RollThrowVariant((*(u8 *)((u8 *)p0 + 0x57c) & 2));
    BossFPartD8_TriggerPartDAThrow();
}
