#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; s32 unk4; } UnkEntry;
typedef struct { u8 unk0[6]; } UnkElem;
extern UnkEntry *data_0207c3a0[];
extern UnkElem data_020761b4[];
UnkElem *PlayerDemoLight_GetColors(s32 a, s32 b);
UnkElem *PlayerDemoLight_GetColorsAlt(s32 a, s32 b) { s32 i = data_0207c3a0[a][b].unk4; if (i == 0) return PlayerDemoLight_GetColors(a, b); return &data_020761b4[i]; }

#pragma thumb reset
