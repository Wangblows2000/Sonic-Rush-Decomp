#include <nitro/types.h>

s32 func_ov000_0235f9b0(void);
void func_ov000_023629cc(s32 a, s32 b);

#pragma thumb on
void func_ov000_02362b28(void) {
    func_ov000_023629cc(1, func_ov000_0235f9b0());
}
#pragma thumb reset
