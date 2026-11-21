#include <nitro/types.h>

typedef struct UnkStruct_022d15f4 {
    u8 pad[0xba8];
    u8 unkBA8;
} UnkStruct_022d15f4;

void Boss2_SelectNextAttack(s32 a);

void Boss2_State_WaitAnimThenSelectMode2(UnkStruct_022d15f4 *s) {
    if (!(s->unkBA8 & 2))
        return;
    Boss2_SelectNextAttack(2);
}
