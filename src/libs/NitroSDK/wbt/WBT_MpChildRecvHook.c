#include <nitro/types.h>

void WBT_ParseChildRecvPacket(u32 c, u32 d);
void WBT_MpChildRecvHook(u32 c, u32 d) { if (c == 0) return; if (d == 0) return; if (d == 0xffff) return; WBT_ParseChildRecvPacket(c, d); }
