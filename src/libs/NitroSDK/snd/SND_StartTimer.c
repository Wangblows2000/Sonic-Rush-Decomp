#include <nitro/types.h>

void PushCommand_impl(u32 cmd, u32 a, u32 b, u32 c, u32 d);
void SND_StartTimer(u32 a, u32 b, u32 c, u32 d) { PushCommand_impl(0xc, a, b, c, d); }
