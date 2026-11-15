#include <nitro/types.h>

void func_02004398(void);
void func_020224bc(void);

void func_02025a60(void) {
    func_02004398();
    func_020224bc();
    *(vu32 *)0x04000600 = (*(vu32 *)0x04000600 & ~0xC0000000) | 0x80000000;
}
