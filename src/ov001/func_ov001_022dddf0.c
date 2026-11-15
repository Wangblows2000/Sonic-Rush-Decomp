#include <nitro/types.h>

void func_ov001_022ddcd4(s32 a, s32 b, s32 c);

#pragma thumb on
void func_ov001_022dddf0(s32 b, s32 c) {
    func_ov001_022ddcd4(0, b, c);
}
#pragma thumb reset
