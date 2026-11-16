#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0xc]; u32 unk10; BOOL (*cb)(void *); } UnkStruct_0206294c;
void TalkDemoScene_State_StartDisplay(void);
void TalkDemoScene_State_Bg1Effect(UnkStruct_0206294c *s) { BOOL (*cb)(void *) = s->cb; void *arg = &s->cb; if (cb == NULL) { s->unk10 = 5; s->fn = TalkDemoScene_State_StartDisplay; return; } if (cb(arg)) { s->unk10 = 5; s->fn = TalkDemoScene_State_StartDisplay; } }

#pragma thumb reset
