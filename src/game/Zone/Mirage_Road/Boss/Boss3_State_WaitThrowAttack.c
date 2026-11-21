#include <nitro/types.h>

BOOL Boss3_IsThrowAttackDone(void);
void Boss3_IdleAll(void);

void Boss3_State_WaitThrowAttack(void) {
    if (Boss3_IsThrowAttackDone() == 0)
        return;
    Boss3_IdleAll();
}
