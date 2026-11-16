#include <nitro/types.h>

typedef struct UnkStruct_022ea33c {
    u8 pad[0x43c];
    void (*func)(void);
    u8 pad1[0x14];
    u16 unk454;
} UnkStruct_022ea33c;

void Boss7PartD5_InputState_Unk_ov000_022ea2d4(void);

s32 Boss7PartD5_InputState_Unk_ov000_022ea33c(UnkStruct_022ea33c *s) {
    if (s->unk454++ > 6)
        s->func = Boss7PartD5_InputState_Unk_ov000_022ea2d4;
    return 0;
}
