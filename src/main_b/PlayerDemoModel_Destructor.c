#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 unk10; } UnkStruct_020569f4;
void PlayerDemoModel_Unk_02056428(u32 a);
void PlayerDemoModel_Destructor(UnkStruct_020569f4 *s) { PlayerDemoModel_Unk_02056428(s->unk10); }

#pragma thumb reset
