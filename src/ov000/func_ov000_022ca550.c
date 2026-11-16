#include <nitro/types.h>

typedef struct UnkStruct_022ca550 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022ca550;

void Model_Release(void *p);
void func_ov000_022fb9c8(UnkStruct_022ca550 *s);

void func_ov000_022ca550(UnkStruct_022ca550 *s) {
    Model_Release(s->unk10 + 0x20c);
    func_ov000_022fb9c8(s);
}
