#include <nitro/types.h>

#pragma thumb on

extern u32 data_02087cb8;
extern u16 *data_02072c78[];
typedef struct { u8 unk0[0x28]; s32 unk28; s32 unk2C; u8 unk30[4]; s32 unk34; } UnkStruct_0204efa8;
void func_0204efa8(UnkStruct_0204efa8 *s) { data_02087cb8 &= ~(0x400 << s->unk34); data_02072c78[s->unk28][s->unk2C * 2] = 0; data_02072c78[s->unk28][s->unk2C * 2 + 1] = 0; }

#pragma thumb reset
