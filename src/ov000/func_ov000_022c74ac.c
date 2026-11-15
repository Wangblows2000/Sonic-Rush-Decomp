#include <nitro/types.h>

typedef struct UnkStruct_022c74ac {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022c74ac;

void func_02031768(void *p);
void func_ov000_022fb9c8(UnkStruct_022c74ac *s);

void func_ov000_022c74ac(UnkStruct_022c74ac *s) {
    func_02031768(s->unk10 + 0x30c);
    func_ov000_022fb9c8(s);
}
