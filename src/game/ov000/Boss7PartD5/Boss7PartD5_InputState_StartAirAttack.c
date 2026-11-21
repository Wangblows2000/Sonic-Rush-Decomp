#include <nitro/types.h>

typedef struct UnkStruct_022ea120 {
    u8 pad[0x58];
    u32 unk58;
    u8 pad1[0x3e0];
    void (*func)(void);
} UnkStruct_022ea120;

void Boss7PartD5_InputState_StartJumpThenAirAttack(void);
void Boss7PartD5_InputState_BeginAirAttackCharge(void);

s32 Boss7PartD5_InputState_StartAirAttack(UnkStruct_022ea120 *s) {
    if (s->unk58 & 1)
        s->func = Boss7PartD5_InputState_StartJumpThenAirAttack;
    else
        s->func = Boss7PartD5_InputState_BeginAirAttackCharge;
    return 0;
}
