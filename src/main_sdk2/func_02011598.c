#include <nitro/types.h>
#include <main_sdk2/wm.h>

void DC_InvalidateRange(void *p, u32 size);

s32 func_02011598(void *cb, u16 aid)
{
    UnkWmWork *w = func_02010b70();
    s32 r = func_02010a50(5, 7, 9, 8, 10, 11);
    u16 state;
    if (r != 0) {
        return r;
    }
    state = *(u16 *)w->unk4;
    if (state == 7 || state == 9) {
        if (aid < 1 || aid > 15) {
            return 6;
        }
        DC_InvalidateRange(w->unk4 + 0x17e, 2);
        if (!(*(u16 *)(w->unk4 + 0x17e) & (1 << aid))) {
            return 7;
        }
    } else {
        if (aid != 0) {
            return 6;
        }
    }
    func_02010cd4(0xd, cb);
    r = func_02010b80(0xd, 1, 1 << aid);
    if (r == 0) {
        r = 2;
    }
    return r;
}
