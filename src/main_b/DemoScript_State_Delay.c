#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[8]; u32 flags; s16 unk10; u8 unk12[6]; s8 unk18; u8 unk19; } UnkStruct_02048550;
void DemoScript_ScheduleNext(UnkStruct_02048550 *s);
void DemoScript_State_Delay(UnkStruct_02048550 *s) { s->unk10--; if (s->unk10 <= 0) { s->unk18++; if (s->unk18 >= s->unk19) { s->flags |= 0x80000000; s->fn = NULL; return; } DemoScript_ScheduleNext(s); } }

#pragma thumb reset
