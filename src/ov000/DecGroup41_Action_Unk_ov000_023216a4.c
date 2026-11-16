#include <nitro/types.h>

typedef struct UnkStruct_023216a4 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x9c - 0x6c];
    u16 unk9C;
} UnkStruct_023216a4;

void DecGroup41_State_Unk_ov000_02321514(void);
void StageObj_SetAnimation(UnkStruct_023216a4 *s, u16 a);

void DecGroup41_Action_Unk_ov000_023216a4(UnkStruct_023216a4 *s) {
    s->func = DecGroup41_State_Unk_ov000_02321514;
    StageObj_SetAnimation(s, s->unk9C + 1);
}
