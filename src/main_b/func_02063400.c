#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 unk10; } UnkStruct_02063400;
void func_02063178(u32 a);
void func_02063400(UnkStruct_02063400 *s) { func_02063178(s->unk10); }

#pragma thumb reset
