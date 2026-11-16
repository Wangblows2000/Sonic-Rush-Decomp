#include <nitro/types.h>

void Boss1Neck_TrackPlayerFast(void);

void Boss1Neck_State_FollowPlayer(void) {
    Boss1Neck_TrackPlayerFast();
}
