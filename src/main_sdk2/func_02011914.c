#include <nitro/types.h>
#include <main_sdk2/wm.h>

s32 func_02011914(void *cb, u32 a)
{
    s32 r = func_02010a50(1, 2);
    if (r != 0) {
        return r;
    }
    func_02010cd4(8, cb);
    r = func_02010b80(8, 1, a);
    if (r == 0) {
        r = 2;
    }
    return r;
}
