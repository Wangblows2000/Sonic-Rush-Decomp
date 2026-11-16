#include <nitro/types.h>

void PushCommand_impl(u32 cmd, u32 a, u32 b, u32 c, u32 d);
void SND_ReadDriverInfo(u32 a) { PushCommand_impl(0x21, a, 0, 0, 0); }
