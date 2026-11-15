#include <nitro/types.h>

double func_0202c634(double x) {
    ((u32 *)&x)[1] &= ~0x80000000;
    return x;
}
