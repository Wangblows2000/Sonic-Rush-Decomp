#include <nitro/types.h>

void G3X_ResetMtxStack_2(void);

#define REG(t, a) (*(volatile t *)(a))

void G3X_ResetMtxStack(void)
{
    while (REG(u32, 0x04000600) & 0x8000000) {
    }
    REG(u32, 0x04000600) |= 0x8000;
    REG(u16, 0x04000060) |= 0x2000;
    REG(u16, 0x04000060) |= 0x1000;
    G3X_ResetMtxStack_2();
    REG(u32, 0x040004a4) = 0x1f0080;
    REG(u32, 0x040004a8) = 0;
    REG(u32, 0x040004ac) = 0;
}
