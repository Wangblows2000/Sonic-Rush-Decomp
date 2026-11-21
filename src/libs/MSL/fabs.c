#include <nitro/types.h>

double fabs(double x) {
    ((u32 *)&x)[1] &= ~0x80000000;
    return x;
}
