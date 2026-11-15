#include <nitro/types.h>

typedef struct UnkStruct_022e29ac {
    u8 pad[0xa6c];
    u8 unkA6C;
} UnkStruct_022e29ac;

void func_ov000_022e1b48(s32 a);

void func_ov000_022e29ac(UnkStruct_022e29ac *s) {
    if (!(s->unkA6C & 2))
        return;
    func_ov000_022e1b48(4);
}
