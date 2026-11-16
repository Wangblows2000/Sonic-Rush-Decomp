#include <nitro/types.h>

typedef struct UnkStruct_02314bb8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02314bb8;

void StageObj_SetAnimation(UnkStruct_02314bb8 *s, s32 a);
void RopeCSpring_State_Idle(void);

void RopeCSpring_Action_Idle(UnkStruct_02314bb8 *s) {
    StageObj_SetAnimation(s, 0);
    s->func = RopeCSpring_State_Idle;
}
