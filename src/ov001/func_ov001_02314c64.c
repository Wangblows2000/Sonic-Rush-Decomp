#include <nitro/types.h>

typedef struct { u8 a; u8 pad[0x57]; } UnkStruct_02314c64;
extern UnkStruct_02314c64 *data_ov001_0231f6a4;

void func_ov001_02314c64(s32 i) {
    data_ov001_0231f6a4[i].a = 0;
}
