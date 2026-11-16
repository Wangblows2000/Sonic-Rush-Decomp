#include <nitro/types.h>

void G3X_SetClearColor(u32 a, u32 b, u16 c, u32 d, BOOL e) {
    u32 v = a | (b << 16) | (d << 24);
    if (e) v |= 0x8000;
    *(vu32 *)0x4000350 = v;
    *(vu16 *)0x4000354 = c;
}
