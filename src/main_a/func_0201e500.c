#include <nitro/types.h>

u32 *func_0201d46c(s32 idx);
u32 func_0201e14c(u32 a, u32 b, u32 c);
u32 func_0201d0f8(u32 idx);

s32 func_0201e500(s32 idx, u32 flags, u32 b, u32 c, u32 *out) {
    u32 *info = func_0201d46c(idx);
    u32 r;
    if (info == NULL) return 3;
    if (flags & 8) {
        r = func_0201e14c(*info, b, c);
        if (r == 0) return 7;
    } else {
        r = func_0201d0f8(*info);
    }
    if (out != NULL) *out = r;
    return 0;
}
