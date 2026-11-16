#include <nitro/types.h>

void func_02008910(u32 cmd, u32 a, u32 b, u32 c, u32 d);

void func_02008ae0(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6, u32 p7, u32 p8, u32 p9, u32 p10)
{
    func_02008910(0xe, p1 | (p9 << 16), p3, p6 | ((p7 << 24) | (p8 << 22)), p5 | ((p4 << 26) | (p2 << 24) | (p10 << 16)));
}
