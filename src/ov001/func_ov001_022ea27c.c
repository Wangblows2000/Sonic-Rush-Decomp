#include <nitro/types.h>

typedef struct { u8 pad[0x10]; s32 idx; } UnkStruct_02087cdc;
extern UnkStruct_02087cdc data_02087cdc;
extern s32 data_02087ce0[];

#pragma thumb on
s32 func_ov001_022ea27c(void) {
    return data_02087ce0[data_02087cdc.idx];
}
#pragma thumb reset
