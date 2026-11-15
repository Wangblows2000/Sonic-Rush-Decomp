#include <nitro/types.h>

void func_ov001_023024d0(void *p, s32 a);

void func_ov001_02302348(u8 *p) {
    func_ov001_023024d0(p, 1);
    *(u32 *)(p + 0x14) &= ~0x1000;
}
