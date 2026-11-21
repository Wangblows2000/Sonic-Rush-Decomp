#include <nitro/types.h>

typedef struct UnkStruct_022ea33c {
    u8 pad[0x43c];
    void (*func)(void);
    u8 pad1[0x14];
    u16 unk454;
} UnkStruct_022ea33c;

void Boss7PartD5_InputState_TapBack(void);

s32 Boss7PartD5_InputState_WaitAfterTurn(UnkStruct_022ea33c *s) {
    if (s->unk454++ > 6)
        s->func = Boss7PartD5_InputState_TapBack;
    return 0;
}
