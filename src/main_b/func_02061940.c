#include <nitro/types.h>

#pragma thumb on

typedef struct { u16 unk0; u16 unk2; s16 unk4; s16 unk6; } UnkStruct_0208922c;
extern UnkStruct_0208922c gDisplayMain;
typedef struct { s32 unk0; s32 unk4; s32 unk8; } UnkStruct_02061940;
BOOL func_02061940(UnkStruct_02061940 *s) { gDisplayMain.unk4 = -(s->unk4 >> 8); gDisplayMain.unk6 = -(s->unk8 >> 8); return TRUE; }

#pragma thumb reset
