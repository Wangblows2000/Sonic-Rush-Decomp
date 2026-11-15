#include <nitro/types.h>

typedef struct UnkStruct_0231aed8 {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_0231aed8;

void func_ov000_0231aec0(UnkStruct_0231aed8 *s);

void func_ov000_0231aed8(UnkStruct_0231aed8 *s) {
    s->unkC--;
    if (s->unkC != 0)
        return;
    func_ov000_0231aec0(s);
}
