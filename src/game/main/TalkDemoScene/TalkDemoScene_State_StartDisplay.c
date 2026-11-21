#include <nitro/types.h>

#pragma thumb on

typedef struct { void *unk0; u8 unk4[0xc]; u32 unk10; } UnkSub;
typedef struct { u8 unk0[0x1c]; UnkSub sub; } UnkCfg;
typedef struct { void (*fn)(void); u8 unk4[8]; u32 unkC; u32 unk10; u8 unk14[0x58]; UnkCfg *unk6C; } UnkStruct_02062910;
void TalkDemo_ClearFlags(s32 i, u32 v);
void TalkDemoScene_State_Display(void);
void TalkDemoScene_State_StartDisplay(UnkStruct_02062910 *s) { UnkSub *p = &s->unk6C->sub; if (p->unk0 != NULL) s->unkC = p->unk10; else s->unkC = 0; TalkDemo_ClearFlags(3, 0x40); TalkDemo_ClearFlags(3, 0x10); s->unk10 = 6; s->fn = TalkDemoScene_State_Display; }

#pragma thumb reset
