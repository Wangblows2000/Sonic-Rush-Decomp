#include <nitro/types.h>

BOOL func_0206d7c8(s32 *p, s32 a, s32 b, s32 c, s16 d, u16 e, u16 f);

void func_ov000_0235a760(s32 *p, s32 a, s32 b, s32 c, s16 d, u16 e, u16 f) {
    s16 old = *p;

    if (func_0206d7c8(p, a, b, c, d, e, f))
        return;
    if (*p < 0x366)
        *p += 0x6cd;
    else
        *p -= 0x6cd;
    func_0206d7c8(p, a, b, c, d, e, f);
    *p = old;
}
