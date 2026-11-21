#include <nitro/types.h>

void PushCommand_impl(u32 cmd, u32 a, u32 b, u32 c, u32 d);
void SND_PrepareSeq(u32 a, u32 b, u32 c, u32 d) { PushCommand_impl(2, a, b, c, d); }
