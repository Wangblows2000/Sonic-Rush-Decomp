#include <nitro/types.h>
#include <main_sdk2/wm.h>

s32 func_02012e48(void *cb, u16 a, u16 b, u16 c, u16 d)
{
    UnkWmWork *w = func_02010b70();
    u16 *buf;
    s32 r = func_02010a50(1, 2);
    if (r != 0) {
        return r;
    }
    func_02010cd4(0x1e, cb);
    buf = w->unkC;
    buf[0] = 0x1e;
    buf[1] = a;
    buf[2] = b;
    buf[3] = c;
    buf[4] = d;
    r = func_02010b80(0x1e, 0);
    if (r == 0) {
        r = 2;
    }
    return r;
}
