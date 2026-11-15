#include <nitro/types.h>

typedef struct UnkStruct_0231a3fc {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_0231a3fc;

void func_ov000_0231a450(UnkStruct_0231a3fc *s);

void func_ov000_0231a3fc(UnkStruct_0231a3fc *s) {
    if (!(s->unk5C & 8))
        return;
    func_ov000_0231a450(s);
}
