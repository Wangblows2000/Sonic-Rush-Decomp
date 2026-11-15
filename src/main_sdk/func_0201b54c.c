#include <nitro/types.h>

void func_02008b98(u32 a, u32 b);
extern u32 data_02083448;
void func_0201b54c(u32 m) { if (m == 0) return; func_02008b98(m, 0); data_02083448 &= ~m; }
