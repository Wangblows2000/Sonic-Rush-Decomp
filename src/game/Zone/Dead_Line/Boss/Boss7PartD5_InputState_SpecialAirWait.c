#include <nitro/types.h>

typedef struct UnkStruct_022e9ef0 {
    u8 pad[0x43c];
    void (*func)(void);
    u8 pad1[0x14];
    u16 unk454;
} UnkStruct_022e9ef0;

void Boss7PartD5_InputState_BeginAirAttackCharge(void);

s32 Boss7PartD5_InputState_SpecialAirWait(UnkStruct_022e9ef0 *s) {
    s->unk454--;
    if (s->unk454 == 0)
        s->func = Boss7PartD5_InputState_BeginAirAttackCharge;
    return 0;
}
