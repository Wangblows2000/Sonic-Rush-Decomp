#include <nitro/types.h>

BOOL Boss1_IsSlamDone(void);
void Boss1_Action_IdleAll(void);

void Boss1_State_WaitSlam(void) {
    if (Boss1_IsSlamDone() == 0)
        return;
    Boss1_Action_IdleAll();
}
