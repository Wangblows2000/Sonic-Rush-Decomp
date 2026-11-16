#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; s32 unk4; u32 unk8; } UnkEntry_020766a4;
typedef struct { u8 unk0[8]; s8 unk8; u8 unk9; u8 unkA; u8 unkB[9]; u32 unk14; } UnkStruct_02056988;
extern UnkEntry_020766a4 data_020766a4[];
void PlayerDemoModel_Unk_02056328(UnkStruct_02056988 *s);
void PlayerDemoModel_Unk_02056908(UnkStruct_02056988 *s);
void PlayerDemoModel_Unk_02056988(UnkStruct_02056988 *s) { UnkEntry_020766a4 *e = &data_020766a4[s->unk8]; s->unk14 = e->unk0; s->unkA = e->unk4; PlayerDemoModel_Unk_02056328(s); PlayerDemoModel_Unk_02056908(s); }

#pragma thumb reset
