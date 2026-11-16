#include <nitro/types.h>

u32 func_0200e6c4(u32 v);

u32 func_0200e4ec(u32 type)
{
    u32 v;
    switch (type) {
    case 1:
        v = 1;
        break;
    case 3:
        v = 2;
        break;
    case 2:
        v = 3;
        break;
    default:
        v = 0;
        break;
    }
    if (v == 0) {
        return 0xffff;
    }
    return func_0200e6c4(v);
}
