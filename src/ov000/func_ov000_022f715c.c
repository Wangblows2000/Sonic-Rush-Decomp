#include <nitro/types.h>

typedef struct UnkStruct_022f715c {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022f715c;

void Model_Release(void *p);
void func_ov000_022fb9c8(UnkStruct_022f715c *s);

void func_ov000_022f715c(UnkStruct_022f715c *s) {
    Model_Release(s->unk10 + 0x218);
    func_ov000_022fb9c8(s);
}
