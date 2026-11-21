#include <nitro/types.h>

typedef struct UnkStruct_02301fc0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02301fc0;

void StageObj_SetAnimation(UnkStruct_02301fc0 *s, s32 a);
void BendStick_State_Wobble(void);

void BendStick_Action_Init(UnkStruct_02301fc0 *s) {
    StageObj_SetAnimation(s, 0);
    s->func = BendStick_State_Wobble;
}
