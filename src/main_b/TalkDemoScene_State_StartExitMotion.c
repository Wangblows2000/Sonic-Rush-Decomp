#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x18]; u32 unk18; } UnkCfg;
typedef struct { void (*fn)(void); u32 flags; u8 unk8[8]; u32 unk10; u8 unk14[0x14]; u8 unk28[0x44]; UnkCfg *unk6C; } UnkStruct_020627f8;
void TalkDemo_SetupBg2ExitMotion(void *p, u8 v);
void TalkDemoScene_State_ExitMotion(void);
void TalkDemoScene_State_StartExitMotion(UnkStruct_020627f8 *s) { TalkDemo_SetupBg2ExitMotion(s->unk28, s->unk6C->unk18); switch ((u8)s->unk6C->unk18) { case 9: case 10: s->flags |= 1; } s->unk10 = 8; s->fn = TalkDemoScene_State_ExitMotion; }

#pragma thumb reset
