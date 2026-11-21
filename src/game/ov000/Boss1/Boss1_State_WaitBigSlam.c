#include <nitro/types.h>

BOOL Boss1_IsBigSlamDone(void);
void Boss1_Action_IdleAll(void);

void Boss1_State_WaitBigSlam(void) {
    if (Boss1_IsBigSlamDone() == 0)
        return;
    Boss1_Action_IdleAll();
}
