#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0; u8 unk1; u16 unk2; } UnkElem_0204864c;
typedef struct { void (*fn)(void); u8 unk4[0x10]; UnkElem_0204864c *unk14; s8 unk18; } UnkStruct_0204864c;
extern s16 gPlayerDemoId;
void PlayerDemo_Start(void);
void DemoScript_State_WaitPlayerDemo(void);
void DemoScript_Op_PlayerDemo(UnkStruct_0204864c *s) { gPlayerDemoId = s->unk14[s->unk18].unk1; PlayerDemo_Start(); s->fn = DemoScript_State_WaitPlayerDemo; }

#pragma thumb reset
