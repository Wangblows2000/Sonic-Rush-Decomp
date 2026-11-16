#include <nitro/types.h>

typedef struct UnkStruct_0230e89c {
    u8 pad[0x30];
    u16 unk30;
    u8 pad2[0x58 - 0x32];
    u32 unk58;
} UnkStruct_0230e89c;

void JetCatapult_State_Idle(UnkStruct_0230e89c *s) {
    if (s->unk58 & 1)
        s->unk30 = 0;
}
