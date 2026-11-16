#include <nitro/types.h>

typedef struct UnkStruct_0235abc8 {
    u8 pad[0x10];
    void *unk10;
} UnkStruct_0235abc8;

void Model_Release(void *p);

void func_ov000_0235abc8(UnkStruct_0235abc8 *s) {
    Model_Release(s->unk10);
}
