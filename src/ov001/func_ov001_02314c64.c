#include <nitro/types.h>

typedef struct { u8 a; u8 pad[0x57]; } UnkStruct_02314c64;
extern UnkStruct_02314c64 *gSpecialStageCameraList;

void func_ov001_02314c64(s32 i) {
    gSpecialStageCameraList[i].a = 0;
}
