#include <nitro/types.h>

void WBT_MakeChildSendPacket(void *a, s32 b);
void WBT_MpChildSendHook(void *a, s32 b) { if (b <= 0) return; if (a == 0) return; WBT_MakeChildSendPacket(a, b); }
