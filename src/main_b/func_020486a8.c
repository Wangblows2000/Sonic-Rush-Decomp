#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0xc]; u16 unk10; } UnkStruct_020486a8;
void func_02048678(void);
void func_020486a8(UnkStruct_020486a8 *s) { s->unk10 = 3; s->fn = func_02048678; }

#pragma thumb reset
