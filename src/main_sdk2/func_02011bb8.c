#include <nitro/types.h>
#include <main_sdk2/wm.h>

void DC_InvalidateRange(void *p, u32 size);

s32 func_02011bb8(void *cb)
{
    UnkWmWork *w = func_02010b70();
    s32 r = func_02010a50(2, 9, 10);
    if (r != 0) {
        return r;
    }
    DC_InvalidateRange(w->unk4 + 0xc, 4);
    if (*(u32 *)(w->unk4 + 0xc) == 0) {
        return 3;
    }
    func_02010cd4(0x10, cb);
    r = func_02010b80(0x10, 0);
    if (r == 0) {
        r = 2;
    }
    return r;
}
