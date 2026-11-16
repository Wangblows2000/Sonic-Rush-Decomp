#include <nitro/types.h>

typedef struct UnkStruct_022caff0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022caff0;

extern s32 gBossStatus[];

void Boss1_State_WaitTripleSlam(void);

void Boss1_Action_WaitTripleSlam(UnkStruct_022caff0 *s) {
    gBossStatus[1] = 2;
    s->func = Boss1_State_WaitTripleSlam;
}
