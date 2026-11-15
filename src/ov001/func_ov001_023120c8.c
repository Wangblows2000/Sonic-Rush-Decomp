#include <nitro/types.h>

void func_ov001_0230d6f4(s32 v);
void func_ov001_0231599c(s32 v);

void func_ov001_023120c8(u8 *p) {
    func_ov001_0230d6f4(0x3000);
    *(s32 *)(p + 0x144) = 0;
    func_ov001_0231599c(0);
}
