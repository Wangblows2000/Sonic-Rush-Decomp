#include <nitro/types.h>

typedef struct UnkStruct_02314a98_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_02314a98_v;

typedef struct UnkStruct_02314a98_e {
    u8 pad0[0x20];
    UnkStruct_02314a98_v pos;
    UnkStruct_02314a98_v target;
    u8 pad38[0x58 - 0x38];
} UnkStruct_02314a98_e;

extern UnkStruct_02314a98_e *gSpecialStageCameraList;

void SpecialStageCamera_UpdateOrbitAngles(UnkStruct_02314a98_e *e);

void SpecialStageCamera_SetTarget(s32 idx, UnkStruct_02314a98_v v) {
    UnkStruct_02314a98_e *e = &gSpecialStageCameraList[idx];

    e->target = v;
    SpecialStageCamera_UpdateOrbitAngles(e);
}
