#include <nitro/types.h>

extern s32 data_02087ccc;
extern u32 data_02087cb8;

void func_ov000_0235a2e8(s32 a);

#pragma thumb on
void func_ov000_0235a580(void) {
    func_ov000_0235a2e8(2);
    data_02087ccc = 1;
    data_02087cb8 |= 0x10;
}
#pragma thumb reset
