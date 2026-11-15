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
void func_02055044(void);
void func_02055028(void);

#pragma thumb on
void func_ov001_022d3410(void) {
    if (data_022c4560.b1d < 2) {
        func_02055044();
    } else {
        func_02055028();
    }
}
#pragma thumb reset
