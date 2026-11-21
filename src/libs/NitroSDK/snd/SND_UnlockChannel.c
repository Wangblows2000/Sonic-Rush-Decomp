#include <nitro/types.h>

void PushCommand_impl(u32 cmd, u32 a, u32 b, u32 c, u32 d);
void SND_UnlockChannel(u32 a, u32 b) { PushCommand_impl(0x1b, a, b, 0, 0); }
