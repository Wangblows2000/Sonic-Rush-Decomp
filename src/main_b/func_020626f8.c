#include <nitro/types.h>

#pragma thumb on

typedef struct UnkStruct_020626f8 { void (*fn)(void); u8 unk4[0x24]; BOOL (*cb)(void *); } UnkStruct_020626f8;
void func_020626e8(void);
void func_020626f8(UnkStruct_020626f8 *s) { if (s->cb(&s->cb)) s->fn = func_020626e8; }

#pragma thumb reset
