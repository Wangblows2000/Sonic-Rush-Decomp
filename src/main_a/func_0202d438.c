#include <nitro/types.h>

typedef struct { u8 unk0[4]; u8 unk4; } UnkStruct_0202d438;
s32 func_0202d4ec(UnkStruct_0202d438 *s, s32 n);
void func_0202d49c(UnkStruct_0202d438 *s, s32 n);

void func_0202d438(UnkStruct_0202d438 *s, s32 n) {
    s32 r;
    if (n <= 0) return;
    if (n >= s->unk4) return;
    r = func_0202d4ec(s, n);
    s->unk4 = n;
    if (r < 0) return;
    func_0202d49c(s, n);
}
