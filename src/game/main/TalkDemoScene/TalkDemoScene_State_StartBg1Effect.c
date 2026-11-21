#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x1c]; u32 unk1C; s32 unk20; u32 unk24; } UnkElem;
typedef struct { u8 unk0[0xc]; u8 unkC; } UnkSub;
typedef struct { void (*fn)(void); u8 unk4[0xc]; u32 unk10; UnkSub sub; u8 unk21[0x47]; void *unk68; u8 unk6C[8]; u32 unk74; } UnkStruct_02062980;
UnkElem *TalkDemo_GetScriptPage(void *p, u8 i);
void TalkDemo_StartBg1Effect(UnkSub *p, u8 v);
void TalkDemoScene_State_Bg1Effect(void);
void TalkDemoScene_State_StartBg1Effect(UnkStruct_02062980 *s) { UnkElem *e = TalkDemo_GetScriptPage(s->unk68, s->unk74); u32 *q = &e->unk1C; UnkSub *p = &s->sub; p->unkC = q[1]; TalkDemo_StartBg1Effect(p, q[2]); s->unk10 = 4; s->fn = TalkDemoScene_State_Bg1Effect; }

#pragma thumb reset
