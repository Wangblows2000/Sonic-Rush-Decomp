#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0xc]; u32 unk10; BOOL (*cb)(void *); } UnkStruct_0206294c;
void func_02062910(void);
void func_0206294c(UnkStruct_0206294c *s) { BOOL (*cb)(void *) = s->cb; void *arg = &s->cb; if (cb == NULL) { s->unk10 = 5; s->fn = func_02062910; return; } if (cb(arg)) { s->unk10 = 5; s->fn = func_02062910; } }

#pragma thumb reset
