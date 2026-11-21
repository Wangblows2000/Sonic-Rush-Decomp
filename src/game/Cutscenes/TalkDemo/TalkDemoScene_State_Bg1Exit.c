#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0x10]; BOOL (*cb)(void *); } UnkStruct_02062834;
void TalkDemoScene_State_StartExitMotion(void);
void TalkDemoScene_State_Bg1Exit(UnkStruct_02062834 *s) { BOOL (*cb)(void *) = s->cb; void *arg = &s->cb; if (cb == NULL) { s->fn = TalkDemoScene_State_StartExitMotion; return; } if (cb(arg)) s->fn = TalkDemoScene_State_StartExitMotion; }

#pragma thumb reset
