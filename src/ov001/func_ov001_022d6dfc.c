#include <nitro/types.h>

typedef struct {
    s32 w0;
    u8 pad4[0xc];
    s32 w10;
    s32 w14;
    s32 w18;
    u8 b1c;
    u8 b1d;
    u16 h1e;
} UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
void func_02055028(void);

#pragma thumb on
void func_ov001_022d6dfc(void) {
    data_022c4560.w10 = 2;
    data_022c4560.w18 &= ~0x20;
    data_022c4560.w18 &= ~0x10;
    data_022c4560.b1c = 0;
    data_022c4560.b1d = 0;
    func_02055028();
}
#pragma thumb reset
