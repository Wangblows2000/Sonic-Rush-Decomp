#include <nitro/types.h>

typedef struct UnkStruct_022dc2b8 {
    u8 pad[0x14];
    s32 unk14;
} UnkStruct_022dc2b8;

void func_ov000_022dc274(UnkStruct_022dc2b8 *s);

void func_ov000_022dc2b8(UnkStruct_022dc2b8 *s) {
    if (s->unk14 < 0x20000)
        return;
    s->unk14 = 0x20000;
    func_ov000_022dc274(s);
}
