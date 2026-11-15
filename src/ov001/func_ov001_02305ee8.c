#include <nitro/types.h>

extern u8 *data_ov001_0231f5a8;
void func_ov001_02305d40(void);

void func_ov001_02305ee8(void) {
    *(void (**)(void))(*(u8 **)(data_ov001_0231f5a8 + 0x10) + 8) = func_ov001_02305d40;
}
