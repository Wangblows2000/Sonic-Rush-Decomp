#include <nitro/types.h>

typedef struct UnkStruct_022fcc1c {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_022fcc1c;

void EnemyPawnGroup_Action_JumpAttack(UnkStruct_022fcc1c *s);

void EnemyPawnGroup_State_JumpAttackWindup(UnkStruct_022fcc1c *s) {
    if (s->unkC != 0) {
        s->unkC--;
        return;
    }
    EnemyPawnGroup_Action_JumpAttack(s);
}
