#include <nitro/types.h>

typedef struct UnkStruct_0230b320 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0230b320;

void StageObj_SetAnimation(UnkStruct_0230b320 *s, s32 a);
void GoalRing_State_Idle(void);

void GoalRing_Action_Idle(UnkStruct_0230b320 *s) {
    StageObj_SetAnimation(s, 0);
    s->func = GoalRing_State_Idle;
}
