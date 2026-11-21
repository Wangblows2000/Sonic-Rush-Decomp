#include <nitro/types.h>

typedef struct UnkStruct_0232e90c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0232e90c;

void Effect_State_FollowParent(void);

void Effect_SetFollowParentState(UnkStruct_0232e90c *s) {
    s->func = Effect_State_FollowParent;
}
