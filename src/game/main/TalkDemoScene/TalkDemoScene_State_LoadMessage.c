#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u32 flags; u8 unk8[0xd4]; u8 unkDC[1]; } UnkStruct_02062a10;
void TalkDemo_ReadMessage(void *a, void *b);
void TalkDemo_LoadMessageBg(void);
void TalkDemoScene_State_StartEnterMotion(void);
void TalkDemoScene_State_LoadMessage(UnkStruct_02062a10 *s) { if (!(s->flags & 0x10)) { TalkDemo_ReadMessage(s, s->unkDC); TalkDemo_LoadMessageBg(); } s->fn = TalkDemoScene_State_StartEnterMotion; }

#pragma thumb reset
