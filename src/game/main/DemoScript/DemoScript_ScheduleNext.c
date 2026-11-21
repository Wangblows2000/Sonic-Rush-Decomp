#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0xc]; u16 unk10; } UnkStruct_020486a8;
void DemoScript_State_Wait(void);
void DemoScript_ScheduleNext(UnkStruct_020486a8 *s) { s->unk10 = 3; s->fn = DemoScript_State_Wait; }

#pragma thumb reset
