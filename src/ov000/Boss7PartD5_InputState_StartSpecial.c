#include <nitro/types.h>

typedef struct UnkStruct_022e9f3c {
    u8 pad[0x58];
    u32 unk58;
    u8 pad1[0x3e0];
    void (*func)(void);
} UnkStruct_022e9f3c;

void Boss7PartD5_InputState_TriggerSpecial(void);
void Boss7PartD5_InputState_StartSpecialAirWait(void);

s32 Boss7PartD5_InputState_StartSpecial(UnkStruct_022e9f3c *s) {
    if (s->unk58 & 1)
        s->func = Boss7PartD5_InputState_TriggerSpecial;
    else
        s->func = Boss7PartD5_InputState_StartSpecialAirWait;
    return 0;
}
