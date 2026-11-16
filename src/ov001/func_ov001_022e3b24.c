#include <nitro/types.h>

void Model_Animate(void *p);

#pragma thumb on
void func_ov001_022e3b24(u8 *p) {
    Model_Animate(p + 0x24);
}
#pragma thumb reset
