#include <nitro/types.h>

typedef struct { u32 r; u32 g; u32 b; } UnkColor_0203492c;
extern vu32 data_0208fe70;
BOOL func_0200f0a0(UnkColor_0203492c *c);

void func_0203492c(void) {
    UnkColor_0203492c c;
    u32 v;
    data_0208fe70 = 0;
    if (func_0200f0a0(&c)) return;
    v = c.b | (c.g << 6) | (c.r << 12);
    data_0208fe70 = v;
    data_0208fe70 = v | (v << 16);
}
