#include <nitro/types.h>
#include <main_sdk2/wmds.h>
#include <main_sdk2/wm.h>

void func_020123bc(void *cb);
void func_0201228c(void *cb);
s32 func_020114f0(u16 port, void *cb, void *arg);

s32 func_0201299c(UnkDS *ds)
{
    u8 *sys = func_02010b70();
    s32 r = func_02010a50(2, 9, 10);
    void *portCb;
    if (r != 0) {
        return r;
    }
    if (ds == NULL) {
        return 6;
    }
    if (ds->aidBitmap == 0 ||
        ((portCb = ((void **)(sys + 0xc0))[ds->port]) != func_020123bc && portCb != func_0201228c)) {
        return 3;
    }
    func_020114f0(ds->port, NULL, NULL);
    ds->aidBitmap = 0;
    ds->state = 0;
    return 0;
}
