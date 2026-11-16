#include <nitro/types.h>

void func_02004180(void);

#define REG(t, a) (*(volatile t *)(a))

void func_02004310(void)
{
    while (REG(u32, 0x04000600) & 0x8000000) {
    }
    REG(u32, 0x04000600) |= 0x8000;
    REG(u16, 0x04000060) |= 0x2000;
    REG(u16, 0x04000060) |= 0x1000;
    func_02004180();
    REG(u32, 0x040004a4) = 0x1f0080;
    REG(u32, 0x040004a8) = 0;
    REG(u32, 0x040004ac) = 0;
}
