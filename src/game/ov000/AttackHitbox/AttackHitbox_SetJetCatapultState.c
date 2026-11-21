#include <nitro/types.h>

typedef struct UnkStruct_0234fad0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0234fad0;

void AttackHitbox_State_JetCatapult(void);

void AttackHitbox_SetJetCatapultState(UnkStruct_0234fad0 *s) {
    s->func = AttackHitbox_State_JetCatapult;
}
