#include <nitro/types.h>

void WBT_Unk_02014e08(void *a, s32 b);
void WBT_MpChildSendHook(void *a, s32 b) { if (b <= 0) return; if (a == 0) return; WBT_Unk_02014e08(a, b); }
