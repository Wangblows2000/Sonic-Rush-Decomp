#include <nitro/types.h>

void func_02014e08(void *a, s32 b);
void WBT_MpChildSendHook(void *a, s32 b) { if (b <= 0) return; if (a == 0) return; func_02014e08(a, b); }
