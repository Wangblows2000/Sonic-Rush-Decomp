#include <nitro/types.h>

u8 *func_ov000_0235cd98(void *p);

#pragma thumb on
void func_ov000_0235ce04(void *p) {
    func_ov000_0235cd98(p)[1] = 0;
}
#pragma thumb reset
