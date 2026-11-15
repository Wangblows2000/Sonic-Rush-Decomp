#include <nitro/types.h>

typedef struct { u8 data[0x48]; } UnkStruct_022e14b8;
void func_ov001_022e13d4(void *p);

#pragma thumb on
void func_ov001_022e14b8(u8 *p) {
    s32 i;
    for (i = 0; i < 0x26; i++) {
        func_ov001_022e13d4(&((UnkStruct_022e14b8 *)(p + 0x360))[i]);
    }
}
#pragma thumb reset
