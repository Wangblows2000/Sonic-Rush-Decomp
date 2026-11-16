#include <nitro/types.h>

typedef struct UnkStruct_022d9d54 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022d9d54;

extern s32 gBossStatus[];

void Boss3_State_WaitPushBall(void);

void Boss3_Action_WaitPushBall(UnkStruct_022d9d54 *s) {
    gBossStatus[1] = 0xe;
    s->func = Boss3_State_WaitPushBall;
}
