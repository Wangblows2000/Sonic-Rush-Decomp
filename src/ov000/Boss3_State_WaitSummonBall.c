#include <nitro/types.h>

BOOL Boss3_IsSummonBallDone(void);
void Boss3_IdleAll(void);

void Boss3_State_WaitSummonBall(void) {
    if (Boss3_IsSummonBallDone() == 0)
        return;
    Boss3_IdleAll();
}
