#include <nitro/types.h>

typedef struct UnkStruct_022d9d08 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022d9d08;

extern s32 gBossStatus[];

void Boss3_State_WaitThrowAttack(void);

void Boss3_Action_WaitThrowAttack(UnkStruct_022d9d08 *s) {
    gBossStatus[1] = 0x11;
    s->func = Boss3_State_WaitThrowAttack;
}
