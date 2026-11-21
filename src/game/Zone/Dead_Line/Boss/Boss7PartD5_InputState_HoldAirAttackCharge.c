#include <nitro/types.h>

typedef struct UnkStruct_022e9f64 {
    u8 pad[0x43c];
    void (*func)(void);
    u8 pad1[0x16];
    s16 unk456;
} UnkStruct_022e9f64;

void Boss7PartD5_InputState_Decide(void);

s32 Boss7PartD5_InputState_HoldAirAttackCharge(UnkStruct_022e9f64 *s) {
    if (s->unk456-- == 0)
        s->func = Boss7PartD5_InputState_Decide;
    return 0x100;
}
