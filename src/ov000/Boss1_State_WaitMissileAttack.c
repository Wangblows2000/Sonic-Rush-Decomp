#include <nitro/types.h>

BOOL Boss1_IsMissileAttackDone(void);
void Boss1_Action_IdleAll(void);

void Boss1_State_WaitMissileAttack(void) {
    if (Boss1_IsMissileAttackDone() == 0)
        return;
    Boss1_Action_IdleAll();
}
