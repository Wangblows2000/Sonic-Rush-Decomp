#include <nitro/types.h>

void WBT_ParseParentRecvPacket(void *c, void *a, u16 b);

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
    WBT_ParseParentRecvPacket(c, a, b);
}
