#include <nitro/types.h>

typedef struct UnkStruct_023433c4 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_023433c4;

void AttackHitbox_State_Boost(void);

void AttackHitbox_SetBoostState(UnkStruct_023433c4 *s) {
    s->func = AttackHitbox_State_Boost;
}
