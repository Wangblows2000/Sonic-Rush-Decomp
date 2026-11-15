#include <nitro/types.h>

u8 *func_ov000_0235cd98(void *p);

#pragma thumb on
void func_ov000_0235ce2c(void *p, u8 v) {
    *func_ov000_0235cd98(p) = v;
}
#pragma thumb reset
