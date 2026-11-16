#include <nitro/types.h>

u32 func_02014f94(void *a, s32 b);
u16 WBT_MpParentSendHook(void *a, s32 b) { u16 r = 0; if (b > 0 && a != 0) r = func_02014f94(a, b); return r; }
