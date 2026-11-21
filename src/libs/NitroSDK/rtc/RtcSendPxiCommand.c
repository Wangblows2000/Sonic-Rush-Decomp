#include <nitro/types.h>

s32 PXI_SendWordByFifo(u32 tag, u32 data, u32 err);
BOOL RtcSendPxiCommand(u32 cmd) { return PXI_SendWordByFifo(5, (cmd << 8) & 0x7f00, 0) >= 0; }
