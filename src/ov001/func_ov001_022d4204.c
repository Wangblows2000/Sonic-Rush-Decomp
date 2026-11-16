#include <nitro/types.h>

extern u8 gPad[];
void Pad_Reset(void *p, s32 a, s32 b);
void func_ov001_022d9650(void);

#pragma thumb on
void func_ov001_022d4204(void) {
    Pad_Reset(gPad, 0, 0);
    func_ov001_022d9650();
}
#pragma thumb reset
