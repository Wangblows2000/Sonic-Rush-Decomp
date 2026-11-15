#include <nitro/types.h>

void func_02050c80(void);
void func_ov001_022c87f8(void);

#pragma thumb on
void func_ov001_022c8cc0(void) {
    func_02050c80();
    func_ov001_022c87f8();
}
#pragma thumb reset
