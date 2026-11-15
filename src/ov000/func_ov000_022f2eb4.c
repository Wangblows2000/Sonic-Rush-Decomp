#include <nitro/types.h>

typedef struct UnkStruct_022f2eb4 {
    u8 pad[0x57c];
    u8 unk57C;
} UnkStruct_022f2eb4;

void func_ov000_022f898c(void);

void func_ov000_022f2eb4(UnkStruct_022f2eb4 *s) {
    if (!(s->unk57C & 2))
        return;
    func_ov000_022f898c();
}
