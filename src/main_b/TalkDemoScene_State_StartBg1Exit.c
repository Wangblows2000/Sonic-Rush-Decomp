#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x28]; u32 unk28; } UnkCfg_02062860;
typedef struct { void (*fn)(void); u8 unk4[8]; u32 unkC; u8 unk10[4]; u8 unk14[0x58]; UnkCfg_02062860 *unk6C; } UnkStruct_02062860;
void TalkDemo_StartBg1Exit(void *p, u8 v);
void TalkDemoScene_State_Bg1Exit(void);
void TalkDemoScene_State_StartBg1Exit(UnkStruct_02062860 *s) { TalkDemo_StartBg1Exit(s->unk14, s->unk6C->unk28); s->unkC = 0; s->fn = TalkDemoScene_State_Bg1Exit; }

#pragma thumb reset
