#include <nitro/types.h>

typedef struct {
    s16 idx;
} UnkState_02055e48;

typedef struct {
    u8 pad0[9];
    u8 act;
} UnkObj_02055e48;

extern UnkState_02055e48 gPlayerDemoId;

u16 *PlayerDemoLight_GetColorsAlt(u8 zone, u8 act);
s16 (*PlayerDemoLight_GetVectorsAlt(u8 zone, u8 act))[3];
void NNS_G3dGlbLightColor(s32 a, u16 b);
void NNS_G3dGlbLightVector(s32 a, s32 x, s32 y, s32 z);

#pragma thumb on
void PlayerDemoLight_ApplyAlt(UnkObj_02055e48 *o) {
    u8 zone = gPlayerDemoId.idx;
    u8 act = o->act;
    u8 i;
    s16 (*p)[3];
    u16 *c;

    c = PlayerDemoLight_GetColorsAlt(zone, act);
    NNS_G3dGlbLightColor(0, c[0]);
    NNS_G3dGlbLightColor(1, c[1]);
    NNS_G3dGlbLightColor(2, c[2]);
    p = PlayerDemoLight_GetVectorsAlt(zone, act);
    for (i = 0; i < 3; i++)
        NNS_G3dGlbLightVector(i, p[i][0], p[i][1], p[i][2]);
}
#pragma thumb reset
