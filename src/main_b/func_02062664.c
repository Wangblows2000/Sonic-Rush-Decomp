#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u32 unk4; u32 unk8; s32 unkC; } UnkStruct_02062664;
void func_02062650(void);
void func_02062664(UnkStruct_02062664 *s) { s->unkC--; if (s->unkC <= 0) s->fn = func_02062650; }

#pragma thumb reset
