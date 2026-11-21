#include <nitro/types.h>

void PushCommand_impl(u32 cmd, u32 a, u32 b, u32 c, u32 d);
void SNDi_SetTrackParam(u32 a, u32 b, u32 c, u32 d, u32 e) { PushCommand_impl(7, a | (e << 24), b, c, d); }
