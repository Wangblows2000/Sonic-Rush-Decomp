#include <nitro/types.h>

extern u32 data_0207efbc;
extern void func_0200f27c(u16 id);
extern void func_0200f25c(u16 id);

u32 func_0200c0f8(void *arc, u32 cmd)
{
    switch (cmd) {
    case 9:
        func_0200f27c((u16)data_0207efbc);
        return 0;
    case 10:
        func_0200f25c((u16)data_0207efbc);
        return 0;
    case 1:
        return 4;
    default:
        return 8;
    }
}
