#include <nitro/types.h>

typedef struct UnkStruct_022dfb34 {
    u8 pad[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 pad1[0x54 - 0x14];
    u32 unk54;
} UnkStruct_022dfb34;

void func_ov000_022dfb34(UnkStruct_022dfb34 *s) {
    s->unk10 += s->unk8;
    s->unkC++;
    if (s->unkC >= 0x14)
        s->unk54 |= 4;
}
