#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0; u8 unk1; u16 unk2; } UnkElem_020485c4;
typedef struct { void (*fn)(void); u8 unk4[0x10]; UnkElem_020485c4 *unk14; s8 unk18; } UnkStruct_020485c4;
extern s16 gTalkDemoId;
void TalkDemo_Create(void);
void DemoScript_State_WaitTalkDemo(void);
void DemoScript_Op_TalkDemo(UnkStruct_020485c4 *s) { gTalkDemoId = s->unk14[s->unk18].unk1; TalkDemo_Create(); s->fn = DemoScript_State_WaitTalkDemo; }

#pragma thumb reset
