#include <nitro/types.h>

void PushCommand_impl(u32 cmd, u32 a, u32 b, u32 c, u32 d);
void SND_SetupCapture(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g) { PushCommand_impl(0x11, c, d, (a << 31) | (b << 30) | (e << 29) | (f << 28) | (g << 27), 0); }
