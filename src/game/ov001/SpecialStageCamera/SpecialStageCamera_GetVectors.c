#include <nitro/types.h>

typedef struct UnkStruct_02315900_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_02315900_v;

typedef struct UnkStruct_02315900_e {
    u8 pad0[0x20];
    UnkStruct_02315900_v pos;
    UnkStruct_02315900_v target;
    UnkStruct_02315900_v up;
    u8 pad44[0x58 - 0x44];
} UnkStruct_02315900_e;

typedef struct UnkStruct_02315900_g {
    u8 pad0[0x34];
    s32 cur;
} UnkStruct_02315900_g;

extern UnkStruct_02315900_g gSpecialStageCamera;
extern UnkStruct_02315900_e *gSpecialStageCameraList;

void SpecialStageCamera_GetVectors(UnkStruct_02315900_v *pos, UnkStruct_02315900_v *target, UnkStruct_02315900_v *up) {
    UnkStruct_02315900_g *c = &gSpecialStageCamera;

    if (pos != NULL)
        *pos = gSpecialStageCameraList[c->cur].pos;
    if (target != NULL)
        *target = gSpecialStageCameraList[c->cur].target;
    if (up != NULL)
        *up = gSpecialStageCameraList[c->cur].up;
}
