#include <nitro/types.h>

typedef struct UnkStruct_022f8200 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022f8200;

void Model_Release(void *p);
void func_ov000_022fb9c8(UnkStruct_022f8200 *s);

void func_ov000_022f8200(UnkStruct_022f8200 *s) {
    Model_Release(s->unk10 + 0x228);
    func_ov000_022fb9c8(s);
}
