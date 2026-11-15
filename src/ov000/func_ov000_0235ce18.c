#include <nitro/types.h>

u8 *func_ov000_0235cd98(void *p);

#pragma thumb on
void func_ov000_0235ce18(void *p) {
    func_ov000_0235cd98(p)[1] = 1;
}
#pragma thumb reset
