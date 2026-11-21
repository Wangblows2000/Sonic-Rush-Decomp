#include <nitro/types.h>

u32 WBT_MakeParentSendPacket(void *a, s32 b);
u16 WBT_MpParentSendHook(void *a, s32 b) { u16 r = 0; if (b > 0 && a != 0) r = WBT_MakeParentSendPacket(a, b); return r; }
