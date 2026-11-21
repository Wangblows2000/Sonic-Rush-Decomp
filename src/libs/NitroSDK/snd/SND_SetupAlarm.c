#include <nitro/types.h>

u8 SNDi_SetAlarmHandler(u32 i, void *cb, void *arg);
void PushCommand_impl(u32 cmd, u32 a, u32 b, u32 c, u32 d);
void SND_SetupAlarm(u32 i, u32 b, u32 c, void *cb, void *arg) { PushCommand_impl(0x12, i, b, c, SNDi_SetAlarmHandler(i, cb, arg)); }
