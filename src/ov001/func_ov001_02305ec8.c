#include <nitro/types.h>

extern u8 *data_ov001_0231f5a8;

void func_ov001_02305ec8(void) {
    *(u16 *)(data_ov001_0231f5a8 + 0x5de) ^= 1;
}
