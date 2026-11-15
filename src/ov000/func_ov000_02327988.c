#include <nitro/types.h>

typedef struct UnkStruct_02327988 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad2[0x1f0 - 0x58];
    u32 unk1F0;
} UnkStruct_02327988;

void func_ov000_02327988(UnkStruct_02327988 *s) {
    if (s->unk1F0 & 2)
        s->unk54 |= 4;
}
