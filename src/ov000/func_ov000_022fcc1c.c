#include <nitro/types.h>

typedef struct UnkStruct_022fcc1c {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_022fcc1c;

void func_ov000_022fcb24(UnkStruct_022fcc1c *s);

void func_ov000_022fcc1c(UnkStruct_022fcc1c *s) {
    if (s->unkC != 0) {
        s->unkC--;
        return;
    }
    func_ov000_022fcb24(s);
}
