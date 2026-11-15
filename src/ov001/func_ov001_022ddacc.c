#include <nitro/types.h>

typedef struct { u8 data[0x2c]; } UnkStruct_022ddacc;
void func_02040ef0(void *p, s32 a, s32 b);

#pragma thumb on
void func_ov001_022ddacc(u8 *p) {
    s32 v = *(s32 *)(p + 0xdc8);
    s32 i;
    for (i = 0; i < 0xe; i++) {
        func_02040ef0(&((UnkStruct_022ddacc *)(p + 0xb44))[i], v, 0);
    }
}
#pragma thumb reset
