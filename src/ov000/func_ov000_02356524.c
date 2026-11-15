#include <nitro/types.h>

u8 *func_020514e4(void *p);

#pragma thumb on
void func_ov000_02356524(void *p, u8 v) {
    u8 *q = func_020514e4(p);
    q[0] = v;
    q[1] = 1;
}
#pragma thumb reset
