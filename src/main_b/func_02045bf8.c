#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x340]; void (*fn)(void); u8 unk344[0x12e8]; u16 unk162C; u16 unk162E; } UnkStruct_02045bf8;
void func_02045b94(void);
void func_02045bf8(UnkStruct_02045bf8 *s) { if (s->unk162C++ > 120) { s->fn = func_02045b94; s->unk162C = 0; s->unk162E = 0; } }

#pragma thumb reset
