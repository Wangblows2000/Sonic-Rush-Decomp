#include <nitro/types.h>

u32 RtcSendPxiCommand(u32 cmd);
u32 RTCi_ReadRawTimeAsync(void) { return RtcSendPxiCommand(0x12); }
