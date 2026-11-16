#include <nitro/types.h>

typedef struct UnkStruct_022d8184 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022d8184;

void Boss3PartCC_State_ThrowAttackJump(void);

void Boss3PartCC_Action_StartThrowAttack(UnkStruct_022d8184 *s) {
    s->func = Boss3PartCC_State_ThrowAttackJump;
}
