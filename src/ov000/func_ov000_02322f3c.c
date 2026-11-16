#include <nitro/types.h>

typedef struct UnkStruct_02322f3c {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_02322f3c;

extern s16 gSwitchTimers;

void func_ov000_02322f3c(UnkStruct_02322f3c *s) {
    if (gSwitchTimers != 0)
        s->unk5C &= ~0x20;
    else
        s->unk5C |= 0x20;
}
