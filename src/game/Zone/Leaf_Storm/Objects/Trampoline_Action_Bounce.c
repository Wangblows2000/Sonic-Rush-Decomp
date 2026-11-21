#include <nitro/types.h>

typedef struct UnkStruct_0231a428 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0231a428;

void StageObj_SetAnimation(UnkStruct_0231a428 *s, s32 a);
void Trampoline_State_Bounce(void);

void Trampoline_Action_Bounce(UnkStruct_0231a428 *s) {
    StageObj_SetAnimation(s, 1);
    s->func = Trampoline_State_Bounce;
}
