#include <nitro/types.h>

u32 RtcSendPxiCommand(u32 cmd);
u32 RTCi_WriteRawStatus2Async(void) { return RtcSendPxiCommand(0x27); }
