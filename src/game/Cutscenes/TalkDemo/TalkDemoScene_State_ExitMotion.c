#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0xc]; u32 unk10; u8 unk14[0x14]; BOOL (*cb)(void *); } UnkStruct_020627c4;
void TalkDemoScene_State_EndPage(void);
void TalkDemoScene_State_ExitMotion(UnkStruct_020627c4 *s) { BOOL (*cb)(void *) = s->cb; void *arg = &s->cb; if (cb == NULL) { s->unk10 = 9; s->fn = TalkDemoScene_State_EndPage; return; } if (cb(arg)) { s->unk10 = 9; s->fn = TalkDemoScene_State_EndPage; } }

#pragma thumb reset
