#include <nitro/types.h>

typedef struct { u8 data[0x48]; } UnkStruct_022d0070;
void func_02032d2c(void *p);

#pragma thumb on
void func_ov001_022d0070(u8 *p) {
    s32 i;
    for (i = 0; i < 0x12; i++) {
        func_02032d2c(&((UnkStruct_022d0070 *)(p + 0x420))[i]);
    }
}
#pragma thumb reset
