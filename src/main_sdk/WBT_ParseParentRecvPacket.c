#include <nitro/types.h>

void *WBT_GetWork(void);
void WBT_ParseRecvPacket(void *a, u32 b, u32 c, s32 d);
void WBT_ParseParentRecvPacket(u32 b, u32 c, s32 d) { if (d <= 0) return; WBT_ParseRecvPacket(WBT_GetWork(), b, c, d); }
