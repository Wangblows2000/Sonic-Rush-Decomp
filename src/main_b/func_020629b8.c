#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0xc]; u32 unk10; u8 unk14[0x14]; BOOL (*cb)(void *); } UnkStruct_020629b8;
void func_02062980(void);
void func_020629b8(UnkStruct_020629b8 *s) { BOOL (*cb)(void *) = s->cb; void *arg = &s->cb; if (cb == NULL) { s->unk10 = 3; s->fn = func_02062980; return; } if (cb(arg)) { s->unk10 = 3; s->fn = func_02062980; } }

#pragma thumb reset
