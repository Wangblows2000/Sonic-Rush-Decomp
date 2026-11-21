#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0xc]; u32 unk10; u8 unk14[0x14]; BOOL (*cb)(void *); } UnkStruct_020629b8;
void TalkDemoScene_State_StartBg1Effect(void);
void TalkDemoScene_State_EnterMotion(UnkStruct_020629b8 *s) { BOOL (*cb)(void *) = s->cb; void *arg = &s->cb; if (cb == NULL) { s->unk10 = 3; s->fn = TalkDemoScene_State_StartBg1Effect; return; } if (cb(arg)) { s->unk10 = 3; s->fn = TalkDemoScene_State_StartBg1Effect; } }

#pragma thumb reset
