#include <nitro/types.h>

BOOL Boss1_IsTripleSlamDone(void);
void Boss1_Action_IdleAll(void);

void Boss1_State_WaitTripleSlam(void) {
    if (Boss1_IsTripleSlamDone() == 0)
        return;
    Boss1_Action_IdleAll();
}
