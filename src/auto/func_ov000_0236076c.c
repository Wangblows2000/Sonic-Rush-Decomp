/* paramclone of func_ov001_022d9cb0 (src/ov001/func_ov001_022d9cb0.c); immediates and relocations remapped. */
#include <nitro/types.h>

void func_0203344c(void *p, s32 a, s32 b);
void func_02032d2c(void *p);

#pragma thumb on
void func_ov000_0236076c(u8 *p, u16 a, u16 b) {
    u8 *q = p + 0x588;
    *(u16 *)(q + 4) = a;
    *(u16 *)(q + 6) = b;
    func_0203344c(q, 0, 0);
    func_02032d2c(q);
}
#pragma thumb reset
