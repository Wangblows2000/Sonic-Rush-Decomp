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
void func_ov001_022d2e3c(s32 a);

#pragma thumb on
void func_ov001_022d301c(void) {
    if (data_022c4560.w0 == 0) {
        func_ov001_022d2e3c(0);
    } else {
        func_ov001_022d2e3c(1);
    }
}
#pragma thumb reset
