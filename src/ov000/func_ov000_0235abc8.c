#include <nitro/types.h>

typedef struct UnkStruct_0235abc8 {
    u8 pad[0x10];
    void *unk10;
} UnkStruct_0235abc8;

void func_02031768(void *p);

void func_ov000_0235abc8(UnkStruct_0235abc8 *s) {
    func_02031768(s->unk10);
}
