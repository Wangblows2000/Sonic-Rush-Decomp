#include <nitro/types.h>

extern u32 data_02083448;
void func_02008bc4(u32 a, u32 b);
BOOL func_0201b584(u32 m) {
    if (m == 0) return 1;
    if (m & data_02083448) return 0;
    func_02008bc4(m, 0);
    data_02083448 |= m;
    return 1;
}
