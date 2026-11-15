#include <nitro/types.h>

typedef struct UnkStruct_0230f4c8 UnkStruct_0230f4c8;
struct UnkStruct_0230f4c8 {
    u8 pad[7];
    s8 unk7;
    u8 pad1[0x5c - 8];
    u16 unk5C;
    u8 pad2[0x80 - 0x5e];
    UnkStruct_0230f4c8 *unk80;
    u8 pad3[0x1bc - 0x84];
    UnkStruct_0230f4c8 *unk1BC;
};

void func_ov000_0230f4c8(UnkStruct_0230f4c8 *s) {
    if (s->unk1BC == NULL)
        return;
    if (s->unk1BC->unk80 != s)
        return;
    if (s->unk5C & 2)
        s->unk7 = -2;
    else
        s->unk7 = 2;
}
