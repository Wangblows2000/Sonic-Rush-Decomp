#include <nitro/types.h>

typedef struct UnkStruct_02327b3c {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x54 - 0x10];
    u32 unk54;
} UnkStruct_02327b3c;

void func_ov000_02327b3c(UnkStruct_02327b3c *s) {
    if (s->unkC-- <= 0)
        s->unk54 |= 4;
}
