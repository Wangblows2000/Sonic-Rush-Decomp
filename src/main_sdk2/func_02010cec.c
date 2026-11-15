#include <nitro/types.h>
#include <main_sdk2/wmsys.h>

extern u16 data_0207fae8;

s32 func_02010b54(void);
s32 func_02010a50(s32 num, ...);
void func_02010694(void);
void func_0200a1b4(u32 fifo, void *cb);

s32 func_02010cec(void)
{
    s32 r;
    if (func_02010b54() != 0) {
        return 3;
    }
    r = func_02010a50(1, 0);
    if (r != 0) {
        return r;
    }
    func_02010694();
    func_0200a1b4(10, NULL);
    data_0207faec = NULL;
    data_0207fae8 = 0;
    return 0;
}
