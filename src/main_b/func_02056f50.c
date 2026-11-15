#include <nitro/types.h>

#pragma thumb on

typedef struct { s32 unk0; s32 unk4; } UnkEntry;
typedef struct { u8 unk0[0x12]; } UnkElem;
typedef struct { u32 unk0; u32 unk4; s8 unk8; u8 unk9[3]; } UnkInfo;
extern UnkEntry *data_0207c3a0[];
extern UnkElem data_020748bc[];
extern UnkElem data_020762c4[];
extern UnkInfo data_020766a4[];
UnkElem *func_02056f50(s32 a, s32 b) { s32 i = data_0207c3a0[a][b].unk0; if (i == 0) { UnkInfo *p = &data_020766a4[a]; return &data_020748bc[p->unk8]; } return &data_020762c4[i]; }

#pragma thumb reset
