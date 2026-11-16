#include <nitro/types.h>
#include <main_sdk2/wm.h>

s32 func_02010f48(void *buf, u16 dma);

s32 WM_Initialize(void *buf, void *cb, u16 dma)
{
    UnkWmWork *w;
    s32 r = func_02010f48(buf, dma);
    if (r != 0) {
        return r;
    }
    func_02010cd4(0, cb);
    w = func_02010b70();
    r = func_02010b80(0, 3, w->unk0, w->unk4, w->unk10);
    if (r == 0) {
        r = 2;
    }
    return r;
}
