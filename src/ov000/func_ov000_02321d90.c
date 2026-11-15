#include <nitro/types.h>

typedef struct UnkStruct_02321d90 {
    u8 pad[0x18];
    void *unk18;
} UnkStruct_02321d90;

void func_ov000_02321cb8(void *a, void *b);

void func_ov000_02321d90(UnkStruct_02321d90 *a, UnkStruct_02321d90 *b) {
    func_ov000_02321cb8(a->unk18, b->unk18);
}
