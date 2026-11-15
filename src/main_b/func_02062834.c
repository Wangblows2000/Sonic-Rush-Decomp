#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0x10]; BOOL (*cb)(void *); } UnkStruct_02062834;
void func_020627f8(void);
void func_02062834(UnkStruct_02062834 *s) { BOOL (*cb)(void *) = s->cb; void *arg = &s->cb; if (cb == NULL) { s->fn = func_020627f8; return; } if (cb(arg)) s->fn = func_020627f8; }

#pragma thumb reset
