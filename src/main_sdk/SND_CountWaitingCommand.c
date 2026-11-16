#include <nitro/types.h>

s32 SND_CountFreeCommand(void);
s32 SND_CountReservedCommand(void);
s32 SND_CountWaitingCommand(void) { s32 a = SND_CountFreeCommand(); return 0x100 - a - SND_CountReservedCommand(); }
