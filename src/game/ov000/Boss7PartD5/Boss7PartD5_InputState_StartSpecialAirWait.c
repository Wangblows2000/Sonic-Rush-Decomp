#include <nitro/types.h>

typedef struct UnkStruct_022e9f1c {
    u8 pad[0x43c];
    void (*func)(void);
    u8 pad1[0x14];
    u16 unk454;
} UnkStruct_022e9f1c;

void Boss7PartD5_InputState_SpecialAirWait(void);

s32 Boss7PartD5_InputState_StartSpecialAirWait(UnkStruct_022e9f1c *s) {
    s->unk454 = 0x78;
    s->func = Boss7PartD5_InputState_SpecialAirWait;
    return 0;
}
