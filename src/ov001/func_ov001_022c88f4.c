#include <nitro/types.h>

void func_0205a33c(void *p);
void func_ov001_022cb6d8(void *p);

#pragma thumb on
void func_ov001_022c88f4(u8 *p) {
    func_0205a33c(p);
    func_ov001_022cb6d8(p + 8);
}
#pragma thumb reset
