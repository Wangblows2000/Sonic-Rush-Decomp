#include <nitro/types.h>

typedef struct UnkStruct_022e282c {
    u8 pad[0xa6c];
    u8 unkA6C;
} UnkStruct_022e282c;

void Boss6_SelectNextAttack(s32 a);

void Boss6_State_HurtWaitSelectAttack(UnkStruct_022e282c *s) {
    if (!(s->unkA6C & 2))
        return;
    Boss6_SelectNextAttack(4);
}
