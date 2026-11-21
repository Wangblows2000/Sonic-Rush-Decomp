#include <nitro/types.h>

s32 abs(s32 x) {
    if (x < 0) x = -x;
    return x;
}
