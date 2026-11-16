#include <nitro/types.h>

typedef struct UnkStruct_02314a48_e {
    u8 pad0[0x48];
    u16 rot;
    u8 pad4A[2];
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_02314a48_e;

extern UnkStruct_02314a48_e *data_ov001_0231f6a4;

void func_ov001_02314a48(s32 idx, u16 rot, s32 x, s32 y, s32 z) {
    data_ov001_0231f6a4[idx].rot = rot;
    data_ov001_0231f6a4[idx].x = x;
    data_ov001_0231f6a4[idx].y = y;
    data_ov001_0231f6a4[idx].z = z;
}
