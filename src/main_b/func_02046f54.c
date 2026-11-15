#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0x62]; u16 unk66; } UnkStruct_02046f54;
void func_02046ef4(void);
void func_02046f54(void *a, UnkStruct_02046f54 *s) { if (s->unk66++ > 15) s->fn = func_02046ef4; }

#pragma thumb reset
