#include <nitro/types.h>

typedef struct UnkStruct_02313ec8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02313ec8;

void StageObj_SetAnimation(UnkStruct_02313ec8 *s, s32 a);
void Rocket_State_Idle(void);

void Rocket_Action_Idle(UnkStruct_02313ec8 *s) {
    StageObj_SetAnimation(s, 0);
    s->func = Rocket_State_Idle;
}
