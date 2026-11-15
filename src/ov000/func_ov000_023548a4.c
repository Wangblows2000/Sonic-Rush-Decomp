#include <nitro/types.h>

typedef struct UnkStruct_023548a4 {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_023548a4;

void func_ov000_023547f0(UnkStruct_023548a4 *s);

void func_ov000_023548a4(UnkStruct_023548a4 *s) {
    if (s->unkC != 0) {
        s->unkC--;
        return;
    }
    func_ov000_023547f0(s);
}
