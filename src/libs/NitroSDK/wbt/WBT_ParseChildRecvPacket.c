#include <nitro/types.h>

void *WBT_GetWork(void);
void WBT_ParseRecvPacket(void *a, u32 b, u32 c, u32 d);
void WBT_ParseChildRecvPacket(u32 c, u32 d) { WBT_ParseRecvPacket(WBT_GetWork(), 0, c, d); }
