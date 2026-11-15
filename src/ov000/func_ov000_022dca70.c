#include <nitro/types.h>

typedef struct UnkStruct_022dca70 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022dca70;

void func_02031768(void *p);
void func_ov000_022fb9c8(UnkStruct_022dca70 *s);

void func_ov000_022dca70(UnkStruct_022dca70 *s) {
    u8 *p = s->unk10;
    func_02031768(p + 0x20c);
    func_02031768(p + 0x344);
    func_ov000_022fb9c8(s);
}
