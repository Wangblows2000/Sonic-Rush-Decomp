#include <nitro/types.h>

typedef struct UnkStruct_0231499c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0231499c;

void StageObj_SetAnimation(UnkStruct_0231499c *s, s32 a);
void RopeCLand_State_Idle(void);

void RopeCLand_Action_Idle(UnkStruct_0231499c *s) {
    StageObj_SetAnimation(s, 0);
    s->func = RopeCLand_State_Idle;
}
