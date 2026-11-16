#include <nitro/types.h>

typedef struct UnkStruct_022f3e9c {
    u8 pad[0x434];
    u16 unk434;
} UnkStruct_022f3e9c;

void BossFPartD8_TrackPlayerX_Slow(UnkStruct_022f3e9c *s);
void BossFPartD8_ChooseAttack(void);

void BossFPartD8_State_WaitNextAttack(UnkStruct_022f3e9c *s) {
    BossFPartD8_TrackPlayerX_Slow(s);
    s->unk434--;
    if (s->unk434 != 0)
        return;
    BossFPartD8_ChooseAttack();
}
