#include <nitro/types.h>

typedef struct UnkStruct_02314a48_e {
    u8 pad0[0x48];
    u16 rot;
    u8 pad4A[2];
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_02314a48_e;

extern UnkStruct_02314a48_e *gSpecialStageCameraList;

void SpecialStageCamera_SetPerspective(s32 idx, u16 rot, s32 x, s32 y, s32 z) {
    gSpecialStageCameraList[idx].rot = rot;
    gSpecialStageCameraList[idx].x = x;
    gSpecialStageCameraList[idx].y = y;
    gSpecialStageCameraList[idx].z = z;
}
