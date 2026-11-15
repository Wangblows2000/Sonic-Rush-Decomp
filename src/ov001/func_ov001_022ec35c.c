#include <nitro/types.h>

typedef struct { s32 a; u8 pad[0x18]; u8 b; u8 c; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
void func_ov001_022ec1e4(s32 a);

#pragma thumb on
void func_ov001_022ec35c(void) {
    data_022c4560.a = 0;
    data_022c4560.b = 0;
    data_022c4560.c = 0;
    func_ov001_022ec1e4(2);
}
#pragma thumb reset
