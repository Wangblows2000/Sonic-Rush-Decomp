#include <nitro/types.h>

void func_02014f2c(u32 c, u32 d);
void WBT_MpChildRecvHook(u32 c, u32 d) { if (c == 0) return; if (d == 0) return; if (d == 0xffff) return; func_02014f2c(c, d); }
