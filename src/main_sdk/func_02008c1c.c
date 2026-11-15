#include <nitro/types.h>

u8 SNDi_SetAlarmHandler(u32 i, void *cb, void *arg);
void func_02008910(u32 cmd, u32 a, u32 b, u32 c, u32 d);
void func_02008c1c(u32 i, u32 b, u32 c, void *cb, void *arg) { func_02008910(0x12, i, b, c, SNDi_SetAlarmHandler(i, cb, arg)); }
