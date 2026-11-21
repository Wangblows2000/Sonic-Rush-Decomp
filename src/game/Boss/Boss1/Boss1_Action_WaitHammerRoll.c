#include <nitro/types.h>

typedef struct UnkStruct_022cb03c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022cb03c;

extern s32 gBossStatus[];

void Boss1_State_WaitHammerRoll(void);

void Boss1_Action_WaitHammerRoll(UnkStruct_022cb03c *s) {
    gBossStatus[1] = 4;
    s->func = Boss1_State_WaitHammerRoll;
}
