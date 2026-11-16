#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0x114]; s32 unk118; u8 unk11C[0x3c]; s16 unk158; } UnkStruct_02046cf4;
void ClearDemo_Unk_02046a54(void);
void ClearDemo_Unk_02046cf4(void *a, UnkStruct_02046cf4 *s) { s->unk158--; if (s->unk158 <= 0) { s->fn = ClearDemo_Unk_02046a54; s->unk118 = 0x1000; } }

#pragma thumb reset
