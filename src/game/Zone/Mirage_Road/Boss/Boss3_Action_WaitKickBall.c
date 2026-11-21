#include <nitro/types.h>

typedef struct UnkStruct_022d9cbc {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022d9cbc;

extern s32 gBossStatus[];

void Boss3_State_WaitKickBall(void);

void Boss3_Action_WaitKickBall(UnkStruct_022d9cbc *s) {
    gBossStatus[1] = 0x10;
    s->func = Boss3_State_WaitKickBall;
}
