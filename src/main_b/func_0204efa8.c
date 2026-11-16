#include <nitro/types.h>

#pragma thumb on

extern u32 gSystemFlags;
extern u16 *gDisplayTable[];
typedef struct { u8 unk0[0x28]; s32 unk28; s32 unk2C; u8 unk30[4]; s32 unk34; } UnkStruct_0204efa8;
void func_0204efa8(UnkStruct_0204efa8 *s) { gSystemFlags &= ~(0x400 << s->unk34); gDisplayTable[s->unk28][s->unk2C * 2] = 0; gDisplayTable[s->unk28][s->unk2C * 2 + 1] = 0; }

#pragma thumb reset
