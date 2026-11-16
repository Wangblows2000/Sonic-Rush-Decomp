#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0x7a]; u16 unk7E; } UnkStruct_02062650;
void TalkDemoScene_State_RockPageNext(void);
void TalkDemoScene_State_RockPageEnd(UnkStruct_02062650 *s) { s->unk7E = 0; s->fn = TalkDemoScene_State_RockPageNext; }

#pragma thumb reset
