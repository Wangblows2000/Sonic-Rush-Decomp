#include <nitro/types.h>

void func_02025268(u32 a, void *b, u32 c);

typedef struct { u32 flags; u8 unk4[0x24]; u8 unk28[0x24]; u8 unk4C[0xC]; } UnkStruct_02028528;

void func_02028528(UnkStruct_02028528 *s) {
    if (!(s->flags & 4)) {
        if (!(s->flags & 2)) {
            func_02025268(0x19, s->unk28, 0xC);
        } else {
            func_02025268(0x1C, s->unk4C, 3);
        }
    } else if (!(s->flags & 2)) {
        func_02025268(0x1A, s->unk28, 9);
    }
    if (s->flags & 1) return;
    func_02025268(0x1B, s->unk4, 3);
}
