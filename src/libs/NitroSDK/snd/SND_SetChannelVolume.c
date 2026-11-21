#include <nitro/types.h>

void PushCommand_impl(u32 cmd, u32 a, u32 b, u32 c, u32 d);
void SND_SetChannelVolume(u32 a, u32 b, u32 c) { PushCommand_impl(0x14, a, b, c, 0); }
