#include <nitro/types.h>

#pragma thumb on

typedef struct { s32 unk0; s32 unk4; s32 unk8; s32 unkC; s32 unk10; } UnkStruct_02061870;
BOOL func_02061940(UnkStruct_02061870 *s);
BOOL func_02061870(UnkStruct_02061870 *s) { UnkStruct_02061870 tmp = {0, 0, 0, 0, 0}; if (s == NULL) s = &tmp; s->unk4 = -0xb000; s->unk8 = -0x5000; func_02061940(s); return TRUE; }

#pragma thumb reset
