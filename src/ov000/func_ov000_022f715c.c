#include <nitro/types.h>

typedef struct UnkStruct_022f715c {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022f715c;

void func_02031768(void *p);
void func_ov000_022fb9c8(UnkStruct_022f715c *s);

void func_ov000_022f715c(UnkStruct_022f715c *s) {
    func_02031768(s->unk10 + 0x218);
    func_ov000_022fb9c8(s);
}
