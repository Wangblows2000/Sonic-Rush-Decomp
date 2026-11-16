#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0x114]; s32 unk118; u8 unk11C[0x3c]; s16 unk158; } UnkStruct_02046cf4;
void ClearDemoModel_State_Move(void);
void ClearDemoModel_State_Delay(void *a, UnkStruct_02046cf4 *s) { s->unk158--; if (s->unk158 <= 0) { s->fn = ClearDemoModel_State_Move; s->unk118 = 0x1000; } }

#pragma thumb reset
