#include <nitro/types.h>

typedef struct UnkStruct_022e29ac {
    u8 pad[0xa6c];
    u8 unkA6C;
} UnkStruct_022e29ac;

void Boss6_SelectNextAttack(s32 a);

void Boss6_State_WaitSelectAttack(UnkStruct_022e29ac *s) {
    if (!(s->unkA6C & 2))
        return;
    Boss6_SelectNextAttack(4);
}
