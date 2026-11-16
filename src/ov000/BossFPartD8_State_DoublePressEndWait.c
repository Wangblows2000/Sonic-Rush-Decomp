#include <nitro/types.h>

typedef struct UnkStruct_022f2eb4 {
    u8 pad[0x57c];
    u8 unk57C;
} UnkStruct_022f2eb4;

void BossF_StartNextAttack(void);

void BossFPartD8_State_DoublePressEndWait(UnkStruct_022f2eb4 *s) {
    if (!(s->unk57C & 2))
        return;
    BossF_StartNextAttack();
}
