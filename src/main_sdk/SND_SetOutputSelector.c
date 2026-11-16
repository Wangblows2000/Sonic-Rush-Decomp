#include <nitro/types.h>

void PushCommand_impl(u32 cmd, u32 a, u32 b, u32 c, u32 d);
void SND_SetOutputSelector(u32 a, u32 b, u32 c, u32 d) { PushCommand_impl(0x19, a, b, c, d); }
