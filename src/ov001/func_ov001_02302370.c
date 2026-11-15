#include <nitro/types.h>

u8 *func_ov001_02300c94(void);
void func_ov001_023024d0(void *p, s32 a);

void func_ov001_02302370(void) {
    u8 *p = func_ov001_02300c94();
    func_ov001_023024d0(p, 7);
    *(u32 *)(p + 0x14) |= 0x1000;
}
