#include <nitro/types.h>

BOOL Boss3_IsHurtDone(void);
void Boss3_IdleAll(void);

void Boss3_State_WaitHurt(void) {
    if (Boss3_IsHurtDone() == 0)
        return;
    Boss3_IdleAll();
}
