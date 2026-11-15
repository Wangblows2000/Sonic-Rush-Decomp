#include <nitro/types.h>

typedef struct UnkStruct_022f7b84 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022f7b84;

void func_02031768(void *p);
void func_ov000_022fb9c8(UnkStruct_022f7b84 *s);

void func_ov000_022f7b84(UnkStruct_022f7b84 *s) {
    func_02031768(s->unk10 + 0x214);
    func_ov000_022fb9c8(s);
}
