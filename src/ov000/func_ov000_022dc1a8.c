#include <nitro/types.h>

typedef struct UnkStruct_022dc1a8 {
    u8 pad[0x348];
    s32 unk348;
} UnkStruct_022dc1a8;

s32 func_ov000_022db848(UnkStruct_022dc1a8 *s);
void func_ov000_022dc188(UnkStruct_022dc1a8 *s);

void func_ov000_022dc1a8(UnkStruct_022dc1a8 *s) {
    if (s->unk348 == func_ov000_022db848(s))
        return;
    func_ov000_022dc188(s);
}
