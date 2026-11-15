#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u32 flags; u8 unk8[0xd4]; u8 unkDC[1]; } UnkStruct_02062a10;
void func_02060fd8(void *a, void *b);
void func_0206200c(void);
void func_020629ec(void);
void func_02062a10(UnkStruct_02062a10 *s) { if (!(s->flags & 0x10)) { func_02060fd8(s, s->unkDC); func_0206200c(); } s->fn = func_020629ec; }

#pragma thumb reset
