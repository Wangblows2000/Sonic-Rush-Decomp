#include <nitro/types.h>

typedef struct UnkStruct_022fdef0 {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_022fdef0;

void func_ov000_022fde64(UnkStruct_022fdef0 *s);

void func_ov000_022fdef0(UnkStruct_022fdef0 *s) {
    s->unkC--;
    if (s->unkC != 0)
        return;
    func_ov000_022fde64(s);
}
