#include <nitro/types.h>

typedef struct UnkStruct_0230b320 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0230b320;

void StageObj_SetAnimation(UnkStruct_0230b320 *s, s32 a);
void GoalRing_State_Unk_ov000_0230b1c8(void);

void func_ov000_0230b320(UnkStruct_0230b320 *s) {
    StageObj_SetAnimation(s, 0);
    s->func = GoalRing_State_Unk_ov000_0230b1c8;
}
