#include <nitro/types.h>

typedef struct UnkStruct_022d4234 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad2[0xba8 - 0x6c];
    u8 unkBA8;
} UnkStruct_022d4234;

void Boss2_State_IdleBeforeAttack3Countdown(void);

void Boss2_State_IdleBeforeAttack3WaitLoop(UnkStruct_022d4234 *s) {
    if (s->unkBA8 & 0x80)
        s->func = Boss2_State_IdleBeforeAttack3Countdown;
}
