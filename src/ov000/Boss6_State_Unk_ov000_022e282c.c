#include <nitro/types.h>

typedef struct UnkStruct_022e282c {
    u8 pad[0xa6c];
    u8 unkA6C;
} UnkStruct_022e282c;

void func_ov000_022e1b48(s32 a);

void Boss6_State_Unk_ov000_022e282c(UnkStruct_022e282c *s) {
    if (!(s->unkA6C & 2))
        return;
    func_ov000_022e1b48(4);
}
