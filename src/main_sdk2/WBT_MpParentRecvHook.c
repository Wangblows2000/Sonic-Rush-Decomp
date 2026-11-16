#include <nitro/types.h>

void func_02014f5c(void *c, void *a, u16 b);

void WBT_MpParentRecvHook(void *a, u16 b, void *c)
{
    if (a == NULL) {
        return;
    }
    if (b == 0) {
        return;
    }
    if (b == 0xffff) {
        return;
    }
    func_02014f5c(c, a, b);
}
