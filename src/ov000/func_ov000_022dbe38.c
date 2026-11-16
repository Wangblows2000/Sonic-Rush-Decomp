#include <nitro/types.h>

typedef struct UnkStruct_022dbe38 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022dbe38;

void Model_Release(void *p);
void func_ov000_022fb9c8(UnkStruct_022dbe38 *s);

void func_ov000_022dbe38(UnkStruct_022dbe38 *s) {
    u8 *p = s->unk10;
    Model_Release(p + 0x20c);
    Model_Release(p + 0x344);
    func_ov000_022fb9c8(s);
}
