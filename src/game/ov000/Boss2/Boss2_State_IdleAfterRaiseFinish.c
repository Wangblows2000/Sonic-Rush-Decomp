#include <nitro/types.h>

typedef struct UnkStruct_022d3fe4 {
    u8 pad[0xba8];
    u8 unkBA8;
} UnkStruct_022d3fe4;

void Boss2_SelectNextAttack(s32 a);

void Boss2_State_IdleAfterRaiseFinish(UnkStruct_022d3fe4 *s) {
    if (!(s->unkBA8 & 2))
        return;
    Boss2_SelectNextAttack(0);
}
