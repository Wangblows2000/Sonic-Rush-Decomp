#include <nitro/types.h>

typedef struct UnkStruct_022dc6cc {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x319 - 0x58];
    u8 unk319;
} UnkStruct_022dc6cc;

void Boss4PartD0_SubState_Unk_ov000_022dc6cc(UnkStruct_022dc6cc *s) {
    if (!(s->unk319 & 2))
        return;
    s->unk54 |= 8;
    s->unk54 |= 2;
}
