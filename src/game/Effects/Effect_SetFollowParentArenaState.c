#include <nitro/types.h>

typedef struct UnkStruct_0232e844 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0232e844;

void Effect_State_FollowParentArena(void);

void Effect_SetFollowParentArenaState(UnkStruct_0232e844 *s) {
    s->func = Effect_State_FollowParentArena;
}
