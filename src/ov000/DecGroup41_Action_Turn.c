#include <nitro/types.h>

typedef struct UnkStruct_023216a4 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x9c - 0x6c];
    u16 unk9C;
} UnkStruct_023216a4;

void DecGroup41_State_Turn(void);
void StageObj_SetAnimation(UnkStruct_023216a4 *s, u16 a);

void DecGroup41_Action_Turn(UnkStruct_023216a4 *s) {
    s->func = DecGroup41_State_Turn;
    StageObj_SetAnimation(s, s->unk9C + 1);
}
