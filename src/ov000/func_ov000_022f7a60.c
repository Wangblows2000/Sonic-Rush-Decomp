#include <nitro/types.h>

typedef struct UnkStruct_022f7a60 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022f7a60;

void Model_Release(void *p);
void func_ov000_022fb9c8(UnkStruct_022f7a60 *s);

void func_ov000_022f7a60(UnkStruct_022f7a60 *s) {
    Model_Release(s->unk10 + 0x23c);
    func_ov000_022fb9c8(s);
}
