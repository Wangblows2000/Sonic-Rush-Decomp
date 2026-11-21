#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x14]; u32 unk14; } UnkCfg_020629ec;
typedef struct { void (*fn)(void); u8 unk4[0xc]; u32 unk10; u8 unk14[0x14]; u8 unk28[0x44]; UnkCfg_020629ec *unk6C; } UnkStruct_020629ec;
void TalkDemo_SetupBg2EnterMotion(void *p, u8 v);
void TalkDemoScene_State_EnterMotion(void);
void TalkDemoScene_State_StartEnterMotion(UnkStruct_020629ec *s) { TalkDemo_SetupBg2EnterMotion(s->unk28, s->unk6C->unk14); s->unk10 = 2; s->fn = TalkDemoScene_State_EnterMotion; }

#pragma thumb reset
