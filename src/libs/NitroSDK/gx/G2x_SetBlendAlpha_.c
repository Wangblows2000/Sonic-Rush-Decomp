#include <nitro/types.h>

void G2x_SetBlendAlpha_(u32 *p, u32 a, u32 b, u32 c, u32 d) { *p = (a | 0x40) | (b << 8) | ((c | (d << 8)) << 16); }
