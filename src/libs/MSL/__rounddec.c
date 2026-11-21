#include <nitro/types.h>

typedef struct { u8 unk0[4]; u8 unk4; } UnkStruct_0202d438;
s32 __must_round(UnkStruct_0202d438 *s, s32 n);
void __dorounddecup(UnkStruct_0202d438 *s, s32 n);

void __rounddec(UnkStruct_0202d438 *s, s32 n) {
    s32 r;
    if (n <= 0) return;
    if (n >= s->unk4) return;
    r = __must_round(s, n);
    s->unk4 = n;
    if (r < 0) return;
    __dorounddecup(s, n);
}
