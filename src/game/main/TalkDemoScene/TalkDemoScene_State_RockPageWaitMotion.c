#include <nitro/types.h>

#pragma thumb on

typedef struct UnkStruct_020626f8 { void (*fn)(void); u8 unk4[0x24]; BOOL (*cb)(void *); } UnkStruct_020626f8;
void TalkDemoScene_State_RockPageDelay(void);
void TalkDemoScene_State_RockPageWaitMotion(UnkStruct_020626f8 *s) { if (s->cb(&s->cb)) s->fn = TalkDemoScene_State_RockPageDelay; }

#pragma thumb reset
