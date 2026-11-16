#include <nitro/types.h>

typedef struct UnkStruct_022dc78c {
    u8 pad[0x228];
    s32 unk228;
    u8 pad1[0x484 - 0x22c];
    s32 unk484;
    u8 pad2[0x48c - 0x488];
    s32 unk48C;
} UnkStruct_022dc78c;

void Boss4PartD0_Unk_ov000_022dc6f4(UnkStruct_022dc78c *s);

void Boss4PartD0_SubState_Unk_ov000_022dc78c(UnkStruct_022dc78c *s) {
    float f = s->unk48C / 5.0f;
    if (f > 1.0f)
        f = 1.0f;
    f = 1.0f - f * (f * f);
    s->unk228 = f * s->unk484;
    if (s->unk48C < 5)
        return;
    Boss4PartD0_Unk_ov000_022dc6f4(s);
}
