#include <nitro/types.h>

typedef struct UnkStruct_022cafa4 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022cafa4;

extern s32 gBossStatus[];

void Boss1_State_WaitMissileAttack(void);

void Boss1_Action_WaitMissileAttack(UnkStruct_022cafa4 *s) {
    gBossStatus[1] = 0x1b;
    s->func = Boss1_State_WaitMissileAttack;
}
