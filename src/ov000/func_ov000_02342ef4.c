#include <nitro/types.h>

typedef struct UnkStruct_02342ef4 {
    u8 pad[0x3c0];
    u16 unk3c0;
} UnkStruct_02342ef4;

void func_ov000_0232f234(UnkStruct_02342ef4 *s);

void func_ov000_02342ef4(UnkStruct_02342ef4 *s) {
    func_ov000_0232f234(s);
    s->unk3c0 = 0x258;
}
