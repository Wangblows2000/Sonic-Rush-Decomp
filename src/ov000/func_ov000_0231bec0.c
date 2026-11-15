#include <nitro/types.h>

typedef struct UnkStruct_0231bec0 {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_0231bec0;

void func_ov000_0231be44(UnkStruct_0231bec0 *s);

void func_ov000_0231bec0(UnkStruct_0231bec0 *s) {
    s->unkC--;
    if (s->unkC != 0)
        return;
    func_ov000_0231be44(s);
}
