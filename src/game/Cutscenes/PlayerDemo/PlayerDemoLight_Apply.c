#include <nitro/types.h>

typedef struct {
    u8 v[8];
} UnkColA_02055eb4;

typedef struct {
    u16 v[4];
} UnkColB_02055eb4;

typedef struct {
    UnkColA_02055eb4 a;
    UnkColB_02055eb4 b;
} UnkParam_02055eb4;

typedef struct {
    UnkColA_02055eb4 a;
    u8 pad[8];
} UnkTabEnt_02055eb4;

typedef struct {
    u8 pad0[8];
    s8 row;
    s8 col;
    u8 padA[2];
} UnkStage_02055eb4;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
} UnkVec_02055eb4;

typedef struct {
    u32 unk0;
    u32 unk4;
} UnkEnt_02055eb4;

typedef struct {
    u8 pad0[9];
    u8 unk9;
} UnkObj_02055eb4;

extern s16 gPlayerDemoId;
extern UnkStage_02055eb4 data_020766a4[];
extern UnkEnt_02055eb4 *data_0207c3a0[];
extern UnkTabEnt_02055eb4 data_02074a04[][3];
extern UnkColB_02055eb4 data_020756dc;

u16 *PlayerDemoLight_GetColors(u8 stage, u8 act);
UnkVec_02055eb4 *PlayerDemoLight_GetVectors(u8 stage, u8 act);
void NNS_G3dGlbLightColor(s32 a, u16 b);
void NNS_G3dGlbLightVector(s32 i, s32 x, s32 y, s32 z);
void G3X_SetEdgeColorTable(UnkParam_02055eb4 *p);

#pragma thumb on
void PlayerDemoLight_Apply(UnkObj_02055eb4 *o) {
    s16 idx = gPlayerDemoId;
    UnkStage_02055eb4 *st = &data_020766a4[idx];
    u8 stage = idx;
    u8 act = o->unk9;
    u16 *c;
    u8 i;
    UnkVec_02055eb4 *v;

    c = PlayerDemoLight_GetColors(stage, act);
    NNS_G3dGlbLightColor(0, c[0]);
    NNS_G3dGlbLightColor(1, c[1]);
    NNS_G3dGlbLightColor(2, c[2]);
    v = PlayerDemoLight_GetVectors(stage, act);
    for (i = 0; i < 3; i++)
        NNS_G3dGlbLightVector(i, v[i].x, v[i].y, v[i].z);
    {
        UnkParam_02055eb4 p = {0};
        s8 row = st->row;
        s8 col = st->col;

        if (data_0207c3a0[stage][act].unk0 == 0)
            { u8 *b = (u8 *)data_02074a04; b += row * 0x30; b += col * 0x10; p.a = *(UnkColA_02055eb4 *)b; }
        { u16 *d = p.b.v; const u16 *s = (const u16 *)(u32)&data_020756dc; d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3]; }
        G3X_SetEdgeColorTable(&p);
    }
}
#pragma thumb reset
