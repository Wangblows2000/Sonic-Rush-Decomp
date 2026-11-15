#include <nitro/types.h>

typedef struct UnkStruct_02342f14 {
    u8 pad[0x3c2];
    u16 unk3c2;
} UnkStruct_02342f14;

void func_ov000_0232f3ec(UnkStruct_02342f14 *s);

void func_ov000_02342f14(UnkStruct_02342f14 *s) {
    func_ov000_0232f3ec(s);
    s->unk3c2 = 0x200;
}
