#include <nitro/types.h>

extern u8 data_022503e0[];
void func_020351d8(void *p, s32 a, s32 b);
void func_ov001_022cf4a0(s32 a);

#pragma thumb on
void func_ov001_022d41e4(void) {
    func_020351d8(data_022503e0, 0, 0);
    func_ov001_022cf4a0(1);
}
#pragma thumb reset
