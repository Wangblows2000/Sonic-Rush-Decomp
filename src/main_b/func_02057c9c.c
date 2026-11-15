#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 unk10; } UnkStruct_02057c9c;
void func_020577c4(u32 a);
void func_02057c9c(UnkStruct_02057c9c *s) { func_020577c4(s->unk10); }

#pragma thumb reset
