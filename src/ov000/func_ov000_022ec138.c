#include <nitro/types.h>

typedef struct UnkStruct_022ec138 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022ec138;

void func_02031768(void *p);
void func_ov000_022fb9c8(UnkStruct_022ec138 *s);

void func_ov000_022ec138(UnkStruct_022ec138 *s) {
    func_02031768(s->unk10 + 0x20c);
    func_ov000_022fb9c8(s);
}
