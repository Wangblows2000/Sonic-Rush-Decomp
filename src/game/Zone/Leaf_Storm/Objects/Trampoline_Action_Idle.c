#include <nitro/types.h>

typedef struct UnkStruct_0231a450 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0231a450;

void StageObj_SetAnimation(UnkStruct_0231a450 *s, s32 a);
void Trampoline_State_Idle(void);

void Trampoline_Action_Idle(UnkStruct_0231a450 *s) {
    StageObj_SetAnimation(s, 0);
    s->func = Trampoline_State_Idle;
}
