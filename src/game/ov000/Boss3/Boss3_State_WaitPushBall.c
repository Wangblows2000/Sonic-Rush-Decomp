#include <nitro/types.h>

BOOL Boss3_IsPushBallDone(void);
void Boss3_IdleAll(void);

void Boss3_State_WaitPushBall(void) {
    if (Boss3_IsPushBallDone() == 0)
        return;
    Boss3_IdleAll();
}
