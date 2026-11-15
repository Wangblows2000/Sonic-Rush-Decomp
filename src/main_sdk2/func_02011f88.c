#include <nitro/types.h>

void *func_02011f4c(void *a, u16 bitmap, void *data, u16 aid);

void *func_02011f88(void *a, u16 *s, u16 aid)
{
    u16 b1 = s[1];
    u32 bit = 1 << aid;
    u16 b0 = s[0];
    if (a == NULL) {
        return NULL;
    }
    if (s == NULL) {
        return NULL;
    }
    if (!(b0 & bit)) {
        return NULL;
    }
    if (!(b1 & bit)) {
        return NULL;
    }
    return func_02011f4c(a, b0, s + 2, aid);
}
