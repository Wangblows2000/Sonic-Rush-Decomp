#include <nitro/types.h>

void func_ov001_022e85c8(void);
void func_ov001_022e2ba4(BOOL on);

#pragma thumb on
void func_ov001_022e8514(void) {
    func_ov001_022e85c8();
    func_ov001_022e2ba4(FALSE);
}
#pragma thumb reset
