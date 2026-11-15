#include <nitro/types.h>

typedef struct UnkStruct_0231db90 {
    u8 pad[0x8];
    s32 unk8;
} UnkStruct_0231db90;

void func_ov000_0231d81c(UnkStruct_0231db90 *s);

void func_ov000_0231db90(UnkStruct_0231db90 *s) {
    if (s->unk8 != 0) {
        s->unk8--;
        return;
    }
    func_ov000_0231d81c(s);
}
