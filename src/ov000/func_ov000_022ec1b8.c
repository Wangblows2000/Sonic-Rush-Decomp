#include <nitro/types.h>

typedef struct UnkStruct_022ec1b8 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad2[4];
    u16 unk5C;
} UnkStruct_022ec1b8;

void func_ov000_022ec1b8(UnkStruct_022ec1b8 *s) {
    if (s->unk5C & 8)
        s->unk54 |= 4;
}
