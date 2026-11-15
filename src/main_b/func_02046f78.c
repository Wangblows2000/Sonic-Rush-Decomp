#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0x62]; u16 unk66; } UnkStruct_02046f78;
void func_02046f54(void);
void func_02046f78(void *a, UnkStruct_02046f78 *s) { s->unk66 = 0; s->fn = func_02046f54; }

#pragma thumb reset
