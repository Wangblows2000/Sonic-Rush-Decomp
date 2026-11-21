#include <nitro/types.h>

BOOL Boss1_IsHammerRollDone(void);
void Boss1_Action_IdleAll(void);

void Boss1_State_WaitHammerRoll(void) {
    if (Boss1_IsHammerRollDone() == 0)
        return;
    Boss1_Action_IdleAll();
}
