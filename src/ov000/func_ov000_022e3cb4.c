#include <nitro/types.h>

typedef struct UnkStruct_022e3cb4 {
    u8 pad[0xa6c];
    u8 unkA6C;
    u8 pad1[0x111c - 0xa6d];
    s32 unk111C;
    s32 unk1120;
} UnkStruct_022e3cb4;

void func_ov000_022e1b48(s32 a);

void func_ov000_022e3cb4(UnkStruct_022e3cb4 *s) {
    if (!(s->unkA6C & 2))
        return;
    func_ov000_022e1b48(3);
    s->unk111C = 0;
    s->unk1120 = 0;
}
