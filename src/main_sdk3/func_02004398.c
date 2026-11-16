#include <nitro/types.h>

void func_020042dc(void);
void func_0200422c(void);
void func_02003f88(void);

#define REG(t, a) (*(volatile t *)(a))

void func_02004398(void)
{
    func_020042dc();
    REG(u32, 0x04000504) = 0;
    while (REG(u32, 0x04000600) & 0x8000000) {
    }
    REG(u16, 0x04000060) = 0;
    REG(u32, 0x04000600) = 0;
    REG(u32, 0x04000010) = 0;
    REG(u16, 0x04000060) |= 0x2000;
    REG(u16, 0x04000060) |= 0x1000;
    REG(u16, 0x04000060) &= ~0x3002;
    REG(u16, 0x04000060) = (u16)((REG(u16, 0x04000060) & ~0x3000) | 0x10);
    REG(u16, 0x04000060) &= (u16)~0x3004;
    REG(u32, 0x04000600) |= 0x8000;
    REG(u32, 0x04000600) = (REG(u32, 0x04000600) & ~0xc0000000) | 0x80000000;
    func_0200422c();
    REG(u32, 0x04000350) = 0;
    REG(u16, 0x04000354) = 0x7fff;
    REG(u16, 0x04000356) = 0;
    REG(u32, 0x04000358) = 0;
    REG(u16, 0x0400035c) = 0;
    REG(u16, 0x04000008) &= ~3;
    func_02003f88();
    REG(u32, 0x040004a4) = 0x1f0080;
    REG(u32, 0x040004a8) = 0;
    REG(u32, 0x040004ac) = 0;
}
