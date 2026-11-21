#include <nitro/types.h>

typedef struct UnkStruct_02331f4c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02331f4c;

void EffBoost_State_FollowPlayer(void);

void EffBoost_SetFollowState(UnkStruct_02331f4c *s) {
    s->func = EffBoost_State_FollowPlayer;
}
