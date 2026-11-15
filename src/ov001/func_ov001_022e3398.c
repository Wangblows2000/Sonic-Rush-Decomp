#include <nitro/types.h>

void func_02053c24(void);
void func_02063f38(void);
void func_020645b0(void);
void func_02054378(void);
void func_02037038(s32 a, s32 b);

#pragma thumb on
void func_ov001_022e3398(void) {
    func_02053c24();
    func_02063f38();
    func_020645b0();
    func_02054378();
    func_02037038(0x3000, 0x3000);
}
#pragma thumb reset
