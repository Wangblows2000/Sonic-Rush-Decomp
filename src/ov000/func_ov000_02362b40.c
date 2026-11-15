#include <nitro/types.h>

s32 func_ov000_0235f9d0(void);
void func_ov000_023629cc(s32 a, s32 b);

#pragma thumb on
void func_ov000_02362b40(void) {
    func_ov000_023629cc(0, func_ov000_0235f9d0());
}
#pragma thumb reset
