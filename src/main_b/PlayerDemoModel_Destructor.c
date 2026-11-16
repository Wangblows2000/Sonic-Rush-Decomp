#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 unk10; } UnkStruct_020569f4;
void PlayerDemoModel_ReleaseScene(u32 a);
void PlayerDemoModel_Destructor(UnkStruct_020569f4 *s) { PlayerDemoModel_ReleaseScene(s->unk10); }

#pragma thumb reset
