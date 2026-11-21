#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 unk4[3]; s32 unk10; } UnkStruct_02087cdc;
typedef struct { u8 unk0[0x34]; s32 unk34; } UnkStruct_0204ef90;
extern UnkStruct_02087cdc data_02087cdc[];
u32 MsgBox_GetScrollBuffer(UnkStruct_0204ef90 *s) { UnkStruct_02087cdc *e = &data_02087cdc[s->unk34]; return e->unk4[e->unk10]; }

#pragma thumb reset
