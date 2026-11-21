#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0xd8]; s16 unkDC; } UnkStruct_02062b7c;
BOOL TalkDemo_ResetBg1Scroll(void *p);
void TalkDemo_ResetBg2Affine(void *p);
void TalkDemo_LoadCommonSprites(UnkStruct_02062b7c *s);
BOOL TalkDemoScene_InitMessageIndex(UnkStruct_02062b7c *s, s16 *out);
void TalkDemoScene_InitSound(UnkStruct_02062b7c *s);
void TalkDemoScene_State_WaitStart(void);
void TalkDemoScene_State_Init(UnkStruct_02062b7c *s) { TalkDemo_ResetBg1Scroll(NULL); TalkDemo_ResetBg2Affine(NULL); TalkDemo_LoadCommonSprites(s); TalkDemoScene_InitMessageIndex(s, &s->unkDC); TalkDemoScene_InitSound(s); s->fn = TalkDemoScene_State_WaitStart; }

#pragma thumb reset
