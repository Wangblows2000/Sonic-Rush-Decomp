#include <nitro/types.h>

typedef struct UnkStruct_0235a980 {
    u8 pad[0x144];
    s32 unk144;
} UnkStruct_0235a980;

void func_ov000_0235ac1c(s32 x, s32 y, s32 z, s32 w);

void func_ov000_0235a980(UnkStruct_0235a980 *s, s32 *v) {
    func_ov000_0235ac1c(v[0] << 4, -v[1] << 4, 0, s->unk144);
}
