#include <nitro/types.h>

typedef struct UnkStruct_02322f3c {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_02322f3c;

extern s16 data_ov000_0236a2bc;

void func_ov000_02322f3c(UnkStruct_02322f3c *s) {
    if (data_ov000_0236a2bc != 0)
        s->unk5C &= ~0x20;
    else
        s->unk5C |= 0x20;
}
