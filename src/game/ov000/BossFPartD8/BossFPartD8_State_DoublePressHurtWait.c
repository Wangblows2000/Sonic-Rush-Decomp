/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossF_UpdateCameraShake();
s32 BossFPartD8_RollThrowVariant();
s32 BossFPartD8_TriggerPartDAThrow();

void BossFPartD8_State_DoublePressHurtWait(u8 *p0) {
    BossF_UpdateCameraShake(((u8 *)p0 + 0x436), 3, 7);
    if ((*(u8 *)((u8 *)p0 + 0x57c) & 2) == 0) {
        return;
    }
    BossFPartD8_RollThrowVariant((*(u8 *)((u8 *)p0 + 0x57c) & 2));
    BossFPartD8_TriggerPartDAThrow();
}
