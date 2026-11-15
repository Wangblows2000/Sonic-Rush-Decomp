#include <nitro/types.h>

typedef struct UnkStruct_022e63ec {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022e63ec;

void func_02031768(void *p);
void func_ov000_022fb9c8(UnkStruct_022e63ec *s);

void func_ov000_022e63ec(UnkStruct_022e63ec *s) {
    func_02031768(s->unk10 + 0x20c);
    func_ov000_022fb9c8(s);
}
