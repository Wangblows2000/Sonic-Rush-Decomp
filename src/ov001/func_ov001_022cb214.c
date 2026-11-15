#include <nitro/types.h>

void func_ov001_022cb364(void *p);

#pragma thumb on
void func_ov001_022cb214(u8 *p) {
    func_ov001_022cb364(p);
    *(s32 *)(p + 0x28) = -1;
}
#pragma thumb reset
