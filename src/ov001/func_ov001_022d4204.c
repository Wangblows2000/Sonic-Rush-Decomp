#include <nitro/types.h>

extern u8 data_022503e0[];
void func_020351d8(void *p, s32 a, s32 b);
void func_ov001_022d9650(void);

#pragma thumb on
void func_ov001_022d4204(void) {
    func_020351d8(data_022503e0, 0, 0);
    func_ov001_022d9650();
}
#pragma thumb reset
