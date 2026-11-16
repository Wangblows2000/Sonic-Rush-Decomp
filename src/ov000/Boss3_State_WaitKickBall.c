#include <nitro/types.h>

BOOL Boss3_IsKickBallDone(void);
void Boss3_IdleAll(void);

void Boss3_State_WaitKickBall(void) {
    if (Boss3_IsKickBallDone() == 0)
        return;
    Boss3_IdleAll();
}
