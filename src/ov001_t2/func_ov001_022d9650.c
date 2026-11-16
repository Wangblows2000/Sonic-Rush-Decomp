#include <nitro/types.h>

extern s32 data_02087ccc;
extern u32 data_02087cb8;

void *func_ov001_022d94b4(void);
void func_ov001_022d7210(void *p, s32 a, s32 b);
void func_ov001_022d79d8(s32 a, s32 b);
void func_ov001_022d74dc(void *p, s32 a);

#pragma thumb on
void func_ov001_022d9650(void) {
    void *p = func_ov001_022d94b4();

    func_ov001_022d7210(p, 0, 0);
    func_ov001_022d79d8(8, 0);
    func_ov001_022d74dc(p, 0);
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1f00;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1f00;
    data_02087ccc = 0;
    data_02087cb8 &= ~0x10;
}
#pragma thumb reset
