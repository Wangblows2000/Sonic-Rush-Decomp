#include <nitro/types.h>

s32 func_02066a9c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
s32 func_02067a28(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

s32 func_02065fa4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
    s32 r = func_02066a9c(a, b, c, d, e, f);
    s32 r2 = func_02067a28(a, b, c, d, e, f);

    if (r > r2)
        r = r2;
    return r;
}
