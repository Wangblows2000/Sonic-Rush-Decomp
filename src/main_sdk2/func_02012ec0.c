#include <nitro/types.h>
#include <main_sdk2/wm.h>

s32 func_02012ec0(void *cb, u32 a, u32 b, u32 c, u16 d)
{
    s32 r = func_02010af8();
    if (r != 0) {
        return r;
    }
    func_02010cd4(0x1d, cb);
    r = func_02010b80(0x1d, 4, a, b, c, d);
    if (r == 0) {
        r = 2;
    }
    return r;
}
