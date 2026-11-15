#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0x24]; u8 unk28[1]; } UnkStruct_02062714;
BOOL func_02061b80(void *a);
void func_020626f8(void);
void func_02062714(UnkStruct_02062714 *s) { func_02061b80(s->unk28); s->fn = func_020626f8; }

#pragma thumb reset
