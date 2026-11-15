#include <nitro/types.h>

extern s32 data_02077964;
extern u32 data_0207d00c;
extern u32 data_0207d008;
void func_02007b3c(s32 a);
void func_020033d8(u32 a);
void func_02004e68(void) {
    if (data_02077964 != -1) func_02007b3c(data_02077964);
    func_020033d8(data_0207d00c);
    data_0207d00c = 0;
    data_0207d008 = 0;
}
