#include <nitro/types.h>

typedef struct UnkStruct_0230a840 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0230a840;

void StageObj_SetAnimation(UnkStruct_0230a840 *s, s32 a);
void GoalRingPart_State_Sparkle(void);

void GoalRingPart_Action_Sparkle(UnkStruct_0230a840 *s) {
    StageObj_SetAnimation(s, 8);
    s->func = GoalRingPart_State_Sparkle;
}
