#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 unk10; } UnkStruct_020569f4;
void func_02056428(u32 a);
void func_020569f4(UnkStruct_020569f4 *s) { func_02056428(s->unk10); }

#pragma thumb reset
